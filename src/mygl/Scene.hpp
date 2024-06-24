#pragma once

#include <glad/glad.h>
#include <vector>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera_3D.hpp"

namespace mygl {
    class Scene {
        public:
            virtual ~Scene(){}
            virtual void store_scene_in_ctx() = 0;
            virtual void open_scene() = 0;
            virtual void close_scene() = 0;
            virtual void update() = 0;
            virtual void scene_clear() = 0;
            virtual void process_input() = 0;
            virtual void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) = 0;
            virtual void left_click_callback(GLFWwindow* window, int button, int action, int mods) = 0;
            virtual void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) = 0;
            virtual void framebuffer_size_callback(GLFWwindow* window, int width, int height) = 0;
    };

    class DefaultScene : public Scene {
        public:
            Context &ctx;

            DefaultScene(Context &ctx) : ctx(ctx)
            {
                store_scene_in_ctx();
            }

            void store_scene_in_ctx() override 
            {
                ctx.scenes.push_back(this);
            }

            void open_scene() override
            {
                glEnable(GL_DEPTH_TEST);
                glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }

            void close_scene() override
            {
                return;
            }

            void update() override 
            {

            }
            void scene_clear() override;
            void process_input() override;
            void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) override;
            void left_click_callback(GLFWwindow* window, int button, int action, int mods) override;
            void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override;
            void framebuffer_size_callback(GLFWwindow* window, int width, int height) override;

        private:
            Camera3D camera;
    };
}