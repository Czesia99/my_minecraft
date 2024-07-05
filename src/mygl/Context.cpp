#include <iostream>
#include "Context.hpp"
#include "Scene.hpp"

namespace mygl
{
    Context::Context(float width, float height, const char *name) {
        win_width = width;
        win_height = height;
        win_name = name;
        window = createWindow();
        loadGlad();
        setCallbacks();
        default_scene = new DefaultScene(*this);
    };

    void Context::setCallbacks()
    {
        glfwSetWindowUserPointer(window, this); //set windowindow pointer
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackWrapper);
        glfwSetCursorPosCallback(window, mouseCallbackWrapper);
        glfwSetMouseButtonCallback(window, leftClickCallbackWrapper);
        glfwSetScrollCallback(window, scrollCallbackWrapper);  
    }

    void Context::loadScene(Scene *scene)
    {
        if (current_scene != nullptr)
            current_scene->closeScene();
        current_scene = scene;
        current_scene->openScene();
        framebufferSizeCallbackWrapper(window, win_width, win_height); //because
    }

    void Context::loadSceneId(int id)
    {
        loadScene(scenes[id]);
    }

    void Context::run()
    {
        if (current_scene == nullptr)
        {
            std::cout << "no scene provided: Loading default scene" << std::endl;
            loadScene(default_scene);
        }

        while (!glfwWindowShouldClose(window))
        {
            current_scene->sceneClear();
            current_scene->processInput();
            current_scene->update();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        // ma_engine_uninit(&sound_manager.engine);
    }

    GLFWwindow *Context::createWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(win_width, win_height, win_name, NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return NULL;
        }
        glfwMakeContextCurrent(window);
        return window;
    };

    int Context::loadGlad() 
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            std::cout << "Can't create context App" << std::endl;
            return -1;
        }
        return 0;
    };

    void Context::framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height) 
    {
        Context* ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));
        ctx->win_width = width;
        ctx->win_height = height;
        ctx->aspect_ratio = width / height;
        if (ctx->current_scene != nullptr) {
            ctx->current_scene->framebufferSizeCallback(window, width, height);
        }   
    }

    void Context::mouseCallbackWrapper(GLFWwindow* window, double x, double y)
    {
        Context* ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));

        float dx = x - ctx->mouse_x;
        float dy = y - ctx->mouse_y;

        ctx->mouse_x = x;
        ctx->mouse_y = y;

        if (ctx->current_scene != nullptr) {
            ctx->current_scene->mouseCallback(window, x, y, dx, dy);
        }
    }

    void Context::scrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset)
    {
        Context* ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));

        if (ctx->current_scene != nullptr) {
            ctx->current_scene->scrollCallback(window, xoffset, yoffset);
        }
    }

    void Context::leftClickCallbackWrapper(GLFWwindow* window, int button, int action, int mods) 
    {
        Context* ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));

        if (ctx->current_scene != nullptr) {
            ctx->current_scene->leftClickCallback(window, button, action, mods);
        }

    }
};
