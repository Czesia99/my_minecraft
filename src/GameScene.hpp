#pragma once

#include "MYGL/Context.hpp"
#include "MYGL/Scene.hpp"
#include "MYGL/Camera3D.hpp"
#include "MYGL/CameraOrtho.hpp"
#include "MYGL/Clock.hpp"
#include "MYGL/Skybox.hpp"
#include "MYGL/Shape.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "Chunk.hpp"
#include "Client.hpp"
#include "glm/gtx/hash.hpp"
#include <unordered_map>
#include <map>
#include <thread>

#include "Blocktype.h"

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

            uint8_t getBlockAt(int x, int y, int z);
            void clearAllChunks();
            void renderWorld(const Shader &shader);

        private:
            void updateChunks();
            void dda();
        private:
            Camera3D camera;
            CameraOrtho camera_ortho;
            Skybox sky;
            Clock clock;
            Chunk *chunk;
            Shader cube_shader;
            Client client;
            GLuint block_textures;

            // Shader test_cube_shader;
            // Cube test_cube;
            Rectangle cursor_img;
            Shader cursor_shader;
            std::unordered_map<glm::ivec3, Chunk*> chunks;
            std::thread client_thread;
            float request_interval = 0.0;
            float peak_rss;
            float current_rss;

            unsigned int depthMapFBO;
            unsigned int depthMap;
            const unsigned int shadow_width = 1024, shadow_height = 1024;
            float near_plane = 0.1f;
            float far_plane = 75.0f;
            glm::mat4 lightProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, near_plane, far_plane);
            glm::mat4 lightView = glm::lookAt(glm::vec3(-10.0f, 30.0f, 0.0f), glm::vec3( 0.0f, 0.0f,  0.0f), glm::vec3( 0.0f, 1.0f,  0.0f));
            glm::mat4 lightSpaceMatrix = lightProjection * lightView;

            Shader cube_shadow;
            Shader depth_shader;
            Shader debug_depth_shader;
            Rectangle depth_quad;

            struct DDA_Data
            {
                uint8_t blocktype;
                float xpos;
                float ypos;
                float zpos;
                glm::ivec3 face;
            };

            DDA_Data dda_data = {};

            std::vector<std::string>block_textures_path = 
            {
                "../assets/textures/grass2.png",
                "../assets/textures/dirt2.png",
                "../assets/textures/stone2.png",
                "../assets/textures/oak_log2.png",
                // "../assets/textures/oak_log.png",
            };

            //EDIT ENUM BLOCKTEXTURES
            std::vector<std::string>block_textures_path2 = 
            {
                "../assets/textures/default/grasstop.jpg",
                "../assets/textures/default/grass_block_side.png",
                "../assets/textures/default/dirt.png",
                "../assets/textures/default/stone.png",
                "../assets/textures/default/oak_log.png",
                "../assets/textures/default/oak_log_top.png",
            };
    };
}