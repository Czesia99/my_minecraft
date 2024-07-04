#include "Scene.hpp"
#include "Context.hpp"
#include "Shape.hpp"
#include "Texture.hpp"
#include "stb_image.h"

namespace mygl
{
    DefaultScene::DefaultScene(Context &ctx) : ctx(ctx), lastX(ctx.win_width / 2), lastY(ctx.win_height / 2)
    {
        // stbi_set_flip_vertically_on_load(true);
        load_texture("../assets/textures/dirt.png", dirt_texture, GL_NEAREST, GL_NEAREST);
        cube.setDiffuseTexture(dirt_texture);
        cube_shader = Shader("cube.vs", "cube.fs");
    }

    void DefaultScene::storeSceneInCtx() {}

    void DefaultScene::openScene()
    {
        glEnable(GL_DEPTH_TEST);
        glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void DefaultScene::closeScene()
    {
        return;
    }

    void DefaultScene::update() 
    {
        cube_shader.use();
        
        

        clock.update();
        sky.render(camera);
        cube.render(cube_shader, camera);
    }

    void DefaultScene::sceneClear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void DefaultScene::processInput()
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

    void DefaultScene::mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (first_mouse)
        {
            lastX = xpos;
            lastY = ypos;
            first_mouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.processMouseMovement(xoffset, yoffset);
    }

    void DefaultScene::leftClickCallback(GLFWwindow* window, int button, int action, int mods)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << "xpos = " << xpos << std::endl;
        std::cout << "ypos = " << ypos << std::endl;
    }

    void DefaultScene::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera.processMouseScroll(static_cast<float>(yoffset));
    }

    void DefaultScene::framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        camera.width = width;
        camera.height = height;
    }
}