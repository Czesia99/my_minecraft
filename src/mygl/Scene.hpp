#pragma once

#include <glad/glad.h>
#include <vector>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera_3D.hpp"
#include "Skybox.hpp"
#include "Clock.hpp"

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
                clock.update();
                sky.render(camera);
            }

            void scene_clear() override
            {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }

            void process_input() override
            {
                if (glfwGetKey(ctx.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(ctx.window, true);

                if (glfwGetKey(ctx.window, GLFW_KEY_W) == GLFW_PRESS)
                    camera.process_keyboard(FORWARD, clock.delta_time);
                if (glfwGetKey(ctx.window, GLFW_KEY_S) == GLFW_PRESS)
                    camera.process_keyboard(BACKWARD, clock.delta_time);
                if (glfwGetKey(ctx.window, GLFW_KEY_A) == GLFW_PRESS)
                    camera.process_keyboard(LEFT, clock.delta_time);
                if (glfwGetKey(ctx.window, GLFW_KEY_D) == GLFW_PRESS)
                    camera.process_keyboard(RIGHT, clock.delta_time);
            }
            void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) override
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

                camera.process_mouse_movement(xoffset, yoffset);
            }

            void left_click_callback(GLFWwindow* window, int button, int action, int mods) override
            {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                std::cout << "xpos = " << xpos << std::endl;
                std::cout << "ypos = " << ypos << std::endl;
            }

            void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) override
            {
                camera.process_mouse_scroll(static_cast<float>(yoffset));
            }

            void framebuffer_size_callback(GLFWwindow* window, int width, int height) override
            {
                glViewport(0, 0, width, height);
                camera.width = width;
                camera.height = height;
            }

        private:
            Camera3D camera;
            Skybox sky;
            Clock clock;

            float lastX = ctx.win_width / 2.0f;
            float lastY = ctx.win_height / 2.0f;
            bool first_mouse = true;
    };
}