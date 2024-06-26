#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>


namespace mygl
{
    class Scene;
    class DefaultScene;
    class Context {
        public:
            Context(float width = 1280, float height = 720, const char *name = "MYGL");

            void set_callbacks();

            void load_scene(Scene *scene);

            void load_scene_id(int id);

            void run();

            GLFWwindow *window;
            float win_width;
            float win_height;
            float aspect_ratio = win_width / win_height;
            const char *win_name; 

            Scene *current_scene = nullptr;
            std::vector<Scene*> scenes;
            DefaultScene *default_scene; // IN CASE NO SCENE
            // Sound sound_manager;
        
        private:
            GLFWwindow *create_window();

            int load_glad();

            static void framebuffer_size_callback_wrapper(GLFWwindow* window, int width, int height);

            static void mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos);

            static void scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset);

            static void left_click_callback_wrapper(GLFWwindow* window, int button, int action, int mods);
    };
}

#endif