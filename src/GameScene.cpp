#include "GameScene.hpp"
#include "MYGL/Texture.hpp"
#include <thread>
#include "memory.hpp"

#include <algorithm>
#include "Blocktype.h"
namespace game 
{
    GameScene::GameScene(Context &ctx) : Scene(ctx)
    {
        storeSceneInCtx();
        if (client.status == -1)
        {
            std::cout << "connexion failed: loading default scene" << std::endl;
            ctx.loadScene(ctx.default_scene);
            ctx.run();
            return;
        }
        std::cout << "in game scene2" << std::endl;
        camera.setCameraSpeed(100.0f);
        camera_ortho = CameraOrtho(glm::vec3(0.0f, 0.0f, 0.0f), ctx.win_width, ctx.win_height);

        loadTextureArray(block_textures_path2, block_textures, 16, 16, GL_NEAREST, GL_NEAREST);
        loadTexture("../assets/cursor.png", cursor_img.texture);

        cube_shader = Shader("cube_shadow.vs", "cube_shadow.fs");
        cube_shadow = Shader("cube_shadow.vs", "cube_shadow.fs");
        cursor_shader = Shader("cursor.vs", "cursor.fs");
        depth_shader = Shader("depth_shader.vs", "depth_shader.fs");
        debug_depth_shader = Shader("debug_depth.vs", "debug_depth.fs");

        if (client.status != -1)
            client_thread = std::thread(&Client::receiveThread, &client);

        cursor_img.transform.scale.x = ctx.win_width;
        cursor_img.transform.scale.y = ctx.win_height;

        glGenFramebuffers(1, &depthMapFBO);
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadow_width, shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        debug_depth_shader.use();
        debug_depth_shader.setInt("depthMap", 0);
        cube_shadow.use();
        cube_shadow.setInt("shadowMap", 1);
    }

    GameScene::~GameScene()
    {
        // printf("%s\n", "game scene destructor");
        // clearAllChunks();
    }

    void GameScene::storeSceneInCtx() 
    {
        ctx.scenes.push_back(this);
    }

    void GameScene::openScene()
    {
        glEnable(GL_DEPTH_TEST);
        glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void GameScene::closeScene()
    {
        return;
    }

    void GameScene::renderWorld(const Shader &shader)
    {
        for (auto &it : chunks)
        {
            it.second->render(shader, camera);
        }
    }

    void GameScene::update() 
    {
        clock.update();

        //render shadow map
        depth_shader.use();
        // glUniformMatrix4fv(depth_shader.setMat4, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        depth_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glViewport(0, 0, shadow_width, shadow_height);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        // ConfigureShaderAndMatrices();
        renderWorld(depth_shader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //reset viewport
        glViewport(0, 0, ctx.win_width, ctx.win_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //render depth map quad
        // debug_depth_shader.use();
        // debug_depth_shader.setFloat("near_plane", near_plane);
        // debug_depth_shader.setFloat("far_plane", far_plane);
        // depth_quad.texture = depthMap;
        // depth_quad.render(debug_depth_shader, camera_ortho);
        
        //render game
        glEnable(GL_CULL_FACE);
        // cube_shader.use();
        cube_shadow.use();
        // cube_shader.use();
        cube_shadow.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        cube_shadow.setVec3("lightPos", glm::vec3(-2.0f, 4.0f, -1.0f));
        cube_shadow.setVec3("viewPos", camera.position);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        sky.render(camera);
        renderWorld(cube_shadow);

        request_interval += clock.delta_time;
        if (request_interval >= 1.0f/20.0f) {
            request_interval = 0;
            client.sendUpdateEntity(camera.position.x, camera.position.y, camera.position.z, camera.yaw, camera.pitch);
        }

        updateChunks();

        for (auto &[key, value] : chunks)
        {
            assert ((key.x == value->chunk_pos.x && key.y == value->chunk_pos.y && key.z == value->chunk_pos.z));
        }
        // glDisable(GL_DEPTH_TEST);
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // cursor_shader.use();
        // cursor_img.render(cursor_shader, camera_ortho);
        // glEnable(GL_DEPTH_TEST);
        // glDisable(GL_BLEND);
    }

    void GameScene::updateChunks()
    {
        if (client.data.chunks.size() != 0)
        {
            //lockmutex
            client.mtx_chunk_data.lock();
            ChunkData chunk = client.data.chunks.front();
            client.data.chunks.pop_front();
            auto it = chunks.find(chunk.pos);
            if (it != chunks.end())
            {
                it->second->deleteChunk();
                delete it->second;
            }
            chunks[chunk.pos] = new Chunk(chunk.pos, chunk.blocktypes);
        }
        client.mtx_chunk_data.unlock();
    }

    void GameScene::dda()
    {
        glm::ivec3 mapPos = glm::ivec3(floor(camera.position));
        glm::vec3 deltaDist = abs(length(camera.front) / camera.front);
        glm::ivec3 rayStep = glm::ivec3(sign(camera.front));
        glm::vec3 sideDist = (sign(camera.front) * (glm::vec3(mapPos) - camera.position) + (sign(camera.front) * glm::vec3(0.5)) + glm::vec3(0.5)) * deltaDist; 
        glm::ivec3 mask;
        
        for (int i = 0; i < 100; i++) {
            uint8_t bt = getBlockAt(mapPos.x, mapPos.y, mapPos.z);
            if (bt != 0)
            {
                dda_data.blocktype = bt;
                dda_data.xpos = mapPos.x;
                dda_data.ypos = mapPos.y;
                dda_data.zpos = mapPos.z;
                dda_data.face = mask;
                return;
            }
            if (sideDist.x < sideDist.y) {
                if (sideDist.x < sideDist.z) {
                    sideDist.x += deltaDist.x;
                    mapPos.x += rayStep.x;
                    mask = glm::ivec3(-rayStep.x, 0, 0);
                }
                else {
                    sideDist.z += deltaDist.z;
                    mapPos.z += rayStep.z;
                    mask = glm::ivec3(0, 0, -rayStep.z);
                }
            }
            else {
                if (sideDist.y < sideDist.z) {
                    sideDist.y += deltaDist.y;
                    mapPos.y += rayStep.y;
                    mask = glm::ivec3(0, -rayStep.y, 0);
                }
                else {
                    sideDist.z += deltaDist.z;
                    mapPos.z += rayStep.z;
                    mask = glm::ivec3(0, 0, -rayStep.z);
                }
            }
        }
    }

    uint8_t GameScene::getBlockAt(int x, int y, int z)
    {
        glm::ivec3 chunk_pos = glm::floor(glm::vec3(x,y,z) / 16.0f) * 16.0f;
        glm::ivec3 local_pos = {x % 16, y % 16, z % 16};
        if (local_pos.x < 0) local_pos.x += 16;
        if (local_pos.y < 0) local_pos.y += 16;
        if (local_pos.z < 0) local_pos.z += 16;

        glm::ivec3 cube_pos = {x, y, z};

        auto it =  chunks.find(chunk_pos);
        if (it != chunks.end())
        {
            uint8_t blocktype = it->second->blocktypes[it->second->positionToIndex(local_pos)];       
            return blocktype;
        } else
            return 0;
    }

    void GameScene::sceneClear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GameScene::processInput()
    {
        if (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(ctx.window, true);

        if (glfwGetKey(ctx.window, GLFW_KEY_W) == GLFW_PRESS) 
        {
            camera.processKeyboard(FORWARD, clock.delta_time);
        }
        if (glfwGetKey(ctx.window, GLFW_KEY_S) == GLFW_PRESS)
        {
            camera.processKeyboard(BACKWARD, clock.delta_time);
        }
        if (glfwGetKey(ctx.window, GLFW_KEY_A) == GLFW_PRESS)
        {
            camera.processKeyboard(LEFT, clock.delta_time);
        }
        if (glfwGetKey(ctx.window, GLFW_KEY_D) == GLFW_PRESS)
        {
            camera.processKeyboard(RIGHT, clock.delta_time);
        }
    }

    void GameScene::mouseCallback(GLFWwindow* window, int x, int y, int dx, int dy)
    {
        camera.processMouseMovement(dx, -dy);
    }

    void GameScene::leftClickCallback(GLFWwindow* window, int button, int action, int mods)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action==GLFW_PRESS)
        {
            dda();
            client.sendUpdateBlock(BlockType::Air, dda_data.xpos, dda_data.ypos, dda_data.zpos);
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action==GLFW_PRESS)
        {
            dda();
            client.sendUpdateBlock(BlockType::Grass, dda_data.xpos + dda_data.face.x, dda_data.ypos + dda_data.face.y, dda_data.zpos + dda_data.face.z);
        }

    }

    void GameScene::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera.processMouseScroll(static_cast<float>(yoffset));
    }

    void GameScene::framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        camera.width = width;
        camera.height = height;
    }

    void GameScene::clearAllChunks()
    {
        for (auto &[key, value] : chunks)
        {
            value->deleteChunk();
            delete value;
        }
        chunks.clear();
    }
}