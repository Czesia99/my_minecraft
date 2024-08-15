#pragma once

#include "MYGL/Context.hpp"
#include "MYGL/Scene.hpp"
#include "MYGL/Camera3D.hpp"
#include "MYGL/Clock.hpp"
#include "MYGL/Skybox.hpp"
#include "MYGL/Shape.hpp"

#include "Chunk.hpp"
#include "Client.hpp"
#include "glm/gtx/hash.hpp"
#include <unordered_map>
#include <thread>

namespace game
{
    using namespace mygl;

    class GameScene : public Scene {
        public:
            GameScene(Context &ctx);
            ~GameScene();
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
            void receiveThread();

            void clearAllChunks();

            void dda();
            uint8_t getBlockAt(int x, int y, int z);

            struct DDA_Data
            {
                uint8_t blocktype;
                float xpos;
                float ypos;
                float zpos;
            };

            DDA_Data dda_data = {};

        private:
            Camera3D camera;
            Skybox sky;
            Clock clock;
            Chunk *chunk;
            Shader cube_shader;
            Client client;
            GLuint block_textures;

            //to test dda
            Shader test_cube_shader;
            Cube test_cube;

            std::vector<std::string>block_textures_path = 
            {
                "../assets/textures/grass2.png",
                "../assets/textures/dirt2.png",
                "../assets/textures/stone2.png",
                "../assets/textures/oak_log2.png",
                // "../assets/textures/oak_log.png",
            };

            // std::vector<std::string>block_textures_path = 
            // {
            //     "../assets/textures/grass2.png",
            //     "../assets/textures/default/dirt.png",
            //     "../assets/textures/default/stone.png",
            //     "../assets/textures/default/oak_log.png",
            // };

            std::unordered_map<glm::ivec3, Chunk*> chunks;
            std::thread t1;
            float request_interval = 0.0;
            float peak_rss;
            float current_rss;
    };
}