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
        glm::vec3 end = camera.front * 8.0f + start;

        float dx = end.x - start.x;
        float dy = end.y - start.y;
        float dz = end.z - start.z;
        
        int steps = std::max({std::abs(dx), std::abs(dy), std::abs(dz)});
        
        int xinc = dx / steps;
        int yinc = dy / steps;
        int zinc = dz / steps;

        int x = (int)start.x;
        int y = (int)start.y;
        int z = (int)start.z;

        for (int i = 0; i <= steps; ++i)
        {
            isBlock(x,y,z);
            std::cout << "Point: (" << (x) << ", " << (y) << ", " << (z) << ")" << std::endl;

            x += xinc;
            y += yinc;
            z += zinc;
        }

    }

    bool GameScene::isBlock(int x, int y, int z)
    {

        int chunk_x = (x < 0 ? (x + 1) / (16 - 1) : x / 16) * 16;
        int chunk_y = (y < 0 ? (y + 1) / (16 - 1) : y / 16) * 16;
        int chunk_z = (z < 0 ? (z + 1) / (16 - 1) : z / 16) * 16;

        std::cout << "chunk pos " << chunk_x << " " << chunk_y << " " << chunk_z << std::endl;

        glm::ivec3 cube_pos = {x, y, z};
        glm::ivec3 chunk_pos = {chunk_x, chunk_y, chunk_z};
        auto it =  chunks.find(chunk_pos);
        if (it != chunks.end())
        {
            int chunktype = (int)it->second->blocktypes[it->second->positionToIndex(cube_pos)];
            std::cout << "chunk found" << std::endl;
            std::cout <<"chunktype == " << chunktype << std::endl;
        } else 
        {
            std::cout << "chunk not found" << std::endl;
        }
        return true;
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