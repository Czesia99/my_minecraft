#include "Context.hpp"

#include <iostream>


#include "Scene.hpp"

namespace mygl
{
    Context::Context(float width, float height, const char *name) {
        win_width = width;
        win_height = height;
        win_name = name;
        window = create_window();
        load_glad();
        set_callbacks();
        default_scene = new DefaultScene(*this);
    };

    void Context::set_callbacks()
    {
        glfwSetWindowUserPointer(window, this); //set windowindow pointer
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_wrapper);
        glfwSetCursorPosCallback(window, mouse_callback_wrapper);
        glfwSetMouseButtonCallback(window, left_click_callback_wrapper);
        glfwSetScrollCallback(window, scroll_callback_wrapper);  
    }

    void Context::load_scene(Scene *scene)
    {
        if (current_scene != nullptr)
            current_scene->close_scene();
        current_scene = scene;
        current_scene->open_scene();
        framebuffer_size_callback_wrapper(window, win_width, win_height); //because
    }

    void Context::load_scene_id(int id)
    {
        load_scene(scenes[id]);
    }

    void Context::run()
    {
        if (current_scene == nullptr)
        {
            current_scene = default_scene;
            // std::cout << "Must assign a current scene" << std::endl; //ASSIGN DEFAULT SCENE
            // return;
        }

        while (!glfwWindowShouldClose(window))
        {
            current_scene->scene_clear();
            current_scene->process_input();
            current_scene->update();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        // ma_engine_uninit(&sound_manager.engine);
    }

    GLFWwindow *Context::create_window() {
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

    int Context::load_glad() 
    {
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            std::cout << "Can't create context App" << std::endl;
            return -1;
        }
        return 0;
    };

    void Context::framebuffer_size_callback_wrapper(GLFWwindow* window, int width, int height) 
    {
        Context* ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));
        ctx->win_width = width;
        ctx->win_height = height;
        ctx->aspect_ratio = width / height;
        if (ctx->current_scene != nullptr) {
            ctx->current_scene->framebuffer_size_callback(window, width, height);
        }   
    }

    void Context::mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos)
    {
        Context* ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));

        if (ctx->current_scene != nullptr) {
            ctx->current_scene->mouse_callback(window, xpos, ypos);
        }
    }

    void Context::scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset)
    {
        Context* ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));

        if (ctx->current_scene != nullptr) {
            ctx->current_scene->scroll_callback(window, xoffset, yoffset);
        }
    }

    void Context::left_click_callback_wrapper(GLFWwindow* window, int button, int action, int mods) 
    {
        Context* ctx = static_cast<Context*>(glfwGetWindowUserPointer(window));

        if (ctx->current_scene != nullptr) {
            ctx->current_scene->left_click_callback(window, button, action, mods);
        }

    }
};
