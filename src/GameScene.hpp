#pragma once

#include "MYGL/Context.hpp"
#include "MYGL/Scene.hpp"
#include "MYGL/Camera3D.hpp"
#include "MYGL/CameraOrtho.hpp"
#include "MYGL/Clock.hpp"
#include "MYGL/Skybox.hpp"
#include "MYGL/Shape.hpp"

#include <unordered_map>
#include <unordered_set>

#include "Entity.hpp"
#include "Client.hpp"
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
            void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) override;
            void mouseCallback(GLFWwindow* window, int x, int y, float dx, float dy);
            void leftClickCallback(GLFWwindow* window, int button, int action, int mods) override;
            void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) override;
            void framebufferSizeCallback(GLFWwindow* window, int width, int height) override;

        private:
            void updateChunks();
            void updateEntities();
            void updateClient();
            //rendering
            void renderCursorQuad();
            void renderSCubeQuad();
            void renderShadowMapQuad();
            void renderShadowMap();
            void renderWorld();
            void renderEntities();
            void renderTerrain(const Shader &shader);
            //dda
            void dda();
            uint8_t getBlockAt(int x, int y, int z);
            uint8_t getBlockAt(float x, float y, float z);
            //player related
            void selectCubeUpdate();
            //utils
            void imguiLayout();
            void clearAllChunks();
            // void createDepthQuadTexture();
        private:
            Camera3D camera;
            CameraOrtho camera_ortho;
            Skybox sky;
            Clock clock;
            Client client;
            GLuint block_textures;

            //Selected Cube
            Camera3D scube_cam = {glm::vec3(0.0f, 0.0f, -5.0f), 200, 200};
            BlockType selected_cube = BlockType::Grass;
            Cube scube;
            float scube_rotspeed = 1.0f;
            Shader scube_shader;
            Shader skybox_shader;

            char input_chat[4096] = {};

            //cursor
            mygl::Rectangle cursor_img;
            Shader cursor_shader;

            //Sky
            mygl::Rectangle sky_img;
            Shader sky_shader;

            std::unordered_map<int, Entity*> entities;

            int cursor_input_mode;
            bool update_chunk = true;

            double request_interval = 0.0;
            // Shader quad_depth_shader;
            // mygl::Rectangle depth_quad;

            struct DDA_Data
            {
                uint8_t blocktype;
                int xpos;
                int ypos;
                int zpos;
                glm::ivec3 face;
            };
            DDA_Data dda_data = {};
    };
}