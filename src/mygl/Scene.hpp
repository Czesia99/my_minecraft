#pragma once

#include <glad/glad.h>
#include <vector>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Context.hpp"
#include "camera_3D.hpp"
#include "Skybox.hpp"
#include "Clock.hpp"

namespace mygl 
{
    class Scene {
        public:
            virtual ~Scene(){}
            virtual void storeSceneInCtx() = 0;
            virtual void openScene() = 0;
            virtual void closeScene() = 0;
            virtual void update() = 0;
            virtual void sceneClear() = 0;
            virtual void processInput() = 0;
            virtual void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) = 0;
            virtual void leftClickCallback(GLFWwindow* window, int button, int action, int mods) = 0;
            virtual void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) = 0;
            virtual void framebufferSizeCallback(GLFWwindow* window, int width, int height) = 0;
    };

    class DefaultScene : public Scene {
        public:
            Context &ctx;
        public:
            DefaultScene(Context &ctx) : ctx(ctx) {}
            
            void storeSceneInCtx() override 
            {
                // ctx.scenes.push_back(*this);
            }

            void openScene() override
            {
                glEnable(GL_DEPTH_TEST);
                glfwSetInputMode(ctx.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }

            void closeScene() override
            {
                return;
            }

            void update() override 
            {
                clock.update();
                sky.render(camera);
            }

            void sceneClear() override
            {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }

            void processInput() override
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
            void mouseCallback(GLFWwindow* window, double xposIn, double yposIn) override
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

            void leftClickCallback(GLFWwindow* window, int button, int action, int mods) override
            {
                double xpos, ypos;
                glfwGetCursorPos(window, &xpos, &ypos);
                std::cout << "xpos = " << xpos << std::endl;
                std::cout << "ypos = " << ypos << std::endl;
            }

            void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) override
            {
                camera.process_mouse_scroll(static_cast<float>(yoffset));
            }

            void framebufferSizeCallback(GLFWwindow* window, int width, int height) override
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