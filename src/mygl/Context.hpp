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
            void setCallbacks();
            void loadScene(Scene *scene);
            void loadSceneId(int id);
            void run();

            GLFWwindow *window;
            float win_width;
            float win_height;
            float aspect_ratio = win_width / win_height;
            const char *win_name;

            Scene *current_scene = nullptr;
            std::vector<Scene*> scenes;
            // Sound sound_manager;
        
        private:
            DefaultScene *default_scene;
        private:
            GLFWwindow *createWindow();
            int loadGlad();
            static void framebufferSizeCallbackWrapper(GLFWwindow* window, int width, int height);
            static void mouseCallbackWrapper(GLFWwindow* window, double xpos, double ypos);
            static void scrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset);
            static void leftClickCallbackWrapper(GLFWwindow* window, int button, int action, int mods);
    };
}

#endif