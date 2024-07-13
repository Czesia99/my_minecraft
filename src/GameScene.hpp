#pragma once

#include "mygl/Context.hpp"
#include "mygl/Scene.hpp"
#include "mygl/Camera3D.hpp"
#include "mygl/Clock.hpp"
#include "mygl/Skybox.hpp"
#include "mygl/Shape.hpp"

#include "Chunk.hpp"
#include "Client.hpp"

namespace game 
{
    using namespace mygl;

    class GameScene : public Scene {
        public:
            GameScene(Context &ctx);
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

            void updateChunks();

        private:
            Camera3D camera;
            Skybox sky;
            Clock clock;
            // Cube cube;
            Chunk *chunk;
            Shader cube_shader;
            GLuint dirt_texture;
            Client client;

            GLuint block_textures;
            std::vector<std::string>block_textures_path = 
            {
                "../assets/textures/dirt.png",
                "../assets/textures/stone.png",
                "../assets/textures/stone.png"
            };

            std::vector<Chunk>chunks;
    };
}