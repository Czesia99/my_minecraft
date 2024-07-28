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
            //delete
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
        //unlock
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
        clearAllChunks();
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