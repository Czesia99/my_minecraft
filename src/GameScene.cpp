#include "GameScene.hpp"
#include "mygl/Texture.hpp"
#include <thread>
#include "memory.hpp"

#include <algorithm>
namespace game 
{
    GameScene::GameScene(Context &ctx) : Scene(ctx)
    {
        storeSceneInCtx();
        // client.receive();
        camera.setCameraSpeed(32.0f);
        loadTextureArray(block_textures_path, block_textures, 16, 16, GL_NEAREST, GL_NEAREST);
        // loadTextureArray(block_textures_path, block_textures, 128, 128);

        cube_shader = Shader("cube.vs", "cube.fs");
        test_cube_shader = Shader("test_cube.vs", "test_cube.fs");
        // test_cube.transform.scale *= 0.5f;
        t1 = std::thread(&Client::receiveThread, &client);
        // t1.detach();
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

    void GameScene::update() 
    {
        glEnable(GL_CULL_FACE);
        clock.update();
        cube_shader.use();
        
        // cube_shader.setInt("material.diffuse", 0);
        glActiveTexture(GL_TEXTURE_2D_ARRAY);
        glBindTextureUnit(GL_TEXTURE_2D_ARRAY, block_textures);


        sky.render(camera);
        // chunk->render(cube_shader, camera);

        for (auto &it : chunks)
        {
            it.second->render(cube_shader, camera);
        }

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
        dda();
        // peak_rss = (double)getPeakRSS() / (1024.0f * 1024.0f  * 1024.0f);
        // current_rss = (double)getCurrentRSS() / (1024.0f * 1024.0f  * 1024.0f);
        // std::cout << "-------- PEAK RSS -------- " << std::endl << peak_rss << std::endl;
        // std::cout << "-------- CURRENT RSS -------- " << std::endl << current_rss << std::endl;
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
                std::cout << "delete chunk" << std::endl;
                it->second->deleteChunk();
                delete it->second;
            }
            chunks[chunk.pos] = new Chunk(chunk.pos, chunk.blocktypes);
        }
        client.mtx_chunk_data.unlock();
    }

    void GameScene::dda()
    {
        glm::vec3 start = camera.position;
        glm::vec3 end = camera.front * 16.0f  + camera.position;
        // isBlock(floor(end.x), floor(end.y), floor(end.z));
        // return;
        
        // std::cout << "END = " << end.x << " " << end.y << " " << end.z << std::endl;
        float dx = end.x - start.x;
        float dy = end.y - start.y;
        float dz = end.z - start.z;
        
        int steps = std::max({std::abs(dx), std::abs(dy), std::abs(dz)});
        
        float xinc = dx / (float)steps;
        float yinc = dy / (float)steps;
        float zinc = dz / (float)steps;

        float x = start.x;
        float y = start.y;
        float z = start.z;

        for (int i = 0; i <= steps; ++i)
        {
            isBlock(floor(x),floor(y),floor(z));
            x += xinc;
            y += yinc;
            z += zinc;
        }
    }

    bool GameScene::isBlock(int x, int y, int z)
    {
        glm::ivec3 chunk_pos = glm::floor(glm::vec3(x,y,z) / 16.0f) * 16.0f;
        glm::ivec3 local_pos = {x % 16, y % 16, z % 16};
        if (local_pos.x < 0) local_pos.x += 16;
        if (local_pos.y < 0) local_pos.y += 16;
        if (local_pos.z < 0) local_pos.z += 16;

        // std::cout << "local pos " << local_pos.x << " " << local_pos.y << " " << local_pos.z << std::endl;
        // std::cout << "chunk pos " << chunk_pos.x << " " << chunk_pos.y << " " << chunk_pos.z << std::endl;

        glm::ivec3 cube_pos = {x, y, z};

        auto it =  chunks.find(chunk_pos);
        if (it != chunks.end())
        {
            int blocktype = it->second->blocktypes[it->second->positionToIndex(local_pos)];
            std::cout <<"blocktype == " << blocktype << std::endl;
            if (blocktype != 0) {
                glDisable(GL_CULL_FACE);
                // test_cube.transform.scale *= 0.5f;
                test_cube_shader.use();
                test_cube_shader.setVec3("isValid", glm::vec3(0.0, 1.0, 0.0));
                test_cube.transform.position = {cube_pos.x, cube_pos.y, cube_pos.z};
                test_cube.render(test_cube_shader, camera);
                return true;
            }
            std::cout << "chunk found" << std::endl;
            
            return false;
            
        } else 
        {
            // glDisable(GL_CULL_FACE);
            // std::cout << "chunk not found" << std::endl;
            // test_cube_shader.use();
            // test_cube_shader.setVec3("isValid", glm::vec3(1.0, 0.0, 0.0));
            // test_cube.transform.position = {cube_pos.x, cube_pos.y, cube_pos.z};
            // test_cube.render(test_cube_shader, camera);
            return false;
        }

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
        // if (action==GLFW_PRESS)
        // {

        //     client.receive();
        //     updateChunks();
        // }
        // std::cout << "xpos = " << xpos << std::endl;
        // std::cout << "ypos = " << ypos << std::endl;
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