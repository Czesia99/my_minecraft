#include "GameScene.hpp"
#include "mygl/Texture.hpp"

namespace game 
{
    GameScene::GameScene(Context &ctx) : Scene(ctx)
    {
        storeSceneInCtx();
        // client.receive();
        camera.setCameraSpeed(4.0f);
        chunk = new Chunk({0, 0, 0}, NULL);
        loadTextureArray(block_textures_path, block_textures, GL_NEAREST, GL_NEAREST);
        cube_shader = Shader("cube.vs", "cube.fs");
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
        cube_shader.use();
        
        cube_shader.setInt("material.diffuse", 0);
        glBindTextureUnit(0, block_textures);


        clock.update();
        sky.render(camera);
        chunk->render(cube_shader, camera);

        updateChunks();        

        // client.receive();
        // std::cout << "x = " << camera.front.x << "y = " << camera.front.y << "z = " <<camera.front.z << std::endl;
    }

    //compare chunks vector size & data chunks size if different create and store chunk
    // void GameScene::updateChunks()
    // {
    //     for (int i = 0; i < client.data->chunks.size(); i++)
    //     {
    //         chunks.push_back(Chunk({client.data->chunks[i].xpos, client.data->chunks[i].ypos, client.data->chunks[i].zpos}, client.data->chunks[i].blockTypes));
    //     }
    // }

    void GameScene::sceneClear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GameScene::processInput()
    {
        if (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(ctx.window, true);

        if (glfwGetKey(ctx.window, GLFW_KEY_W) == GLFW_PRESS)
            camera.processKeyboard(FORWARD, clock.delta_time);
        if (glfwGetKey(ctx.window, GLFW_KEY_S) == GLFW_PRESS)
            camera.processKeyboard(BACKWARD, clock.delta_time);
        if (glfwGetKey(ctx.window, GLFW_KEY_A) == GLFW_PRESS)
            camera.processKeyboard(LEFT, clock.delta_time);
        if (glfwGetKey(ctx.window, GLFW_KEY_D) == GLFW_PRESS)
            camera.processKeyboard(RIGHT, clock.delta_time);
    }

    void GameScene::mouseCallback(GLFWwindow* window, int x, int y, int dx, int dy)
    {
        camera.processMouseMovement(dx, -dy);
    }

    void GameScene::leftClickCallback(GLFWwindow* window, int button, int action, int mods)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        client.receive();
        std::cout << "xpos = " << xpos << std::endl;
        std::cout << "ypos = " << ypos << std::endl;
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
}