#pragma once

#include <glad/glad.h>
#include <vector>

#include <GLFW/glfw3.h>

#include "Camera3D.hpp"
#include "Clock.hpp"
#include "Skybox.hpp"
#include "Shape.hpp"

namespace mygl 
{
    class Context;

    class Scene {
        public:
            Context &ctx;
        public:
            Scene(Context &ctx) : ctx(ctx) {};
            virtual ~Scene() {}
            virtual void storeSceneInCtx() = 0;
            virtual void openScene() = 0;
            virtual void closeScene() = 0;
            virtual void update() = 0;
            virtual void sceneClear() = 0;
            virtual void processInput() = 0;
            virtual void mouseCallback(GLFWwindow* window, int x, int y, int dx, int dy) = 0;
            virtual void leftClickCallback(GLFWwindow* window, int button, int action, int mods) = 0;
            virtual void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) = 0;
            virtual void framebufferSizeCallback(GLFWwindow* window, int width, int height) = 0;
    };

    class DefaultScene : public Scene {
        public:
            // Context &ctx;
        public:
            DefaultScene(Context &ctx);
            void storeSceneInCtx() override;
            void openScene() override;
            void closeScene() override;
            void update() override;
            void sceneClear() override;
            void processInput() override;
            void mouseCallback(GLFWwindow* window, int x, int y, int dx, int dy);
            void leftClickCallback(GLFWwindow* window, int button, int action, int mods) override;
            void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) override;
            void framebufferSizeCallback(GLFWwindow* window, int width, int height) override;

        private:
            Camera3D camera;
            Skybox sky;
            Clock clock;
            Cube cube;
            Shader cube_shader;
            GLuint dirt_texture;

            float lastX;
            float lastY;
            bool first_mouse = true;
    };
}