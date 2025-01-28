#pragma once

#include "MYGL/Camera3D.hpp"
#include "MYGL/Texture.hpp"

#include <unordered_map>
#include <shared_mutex>

#include "glm/gtx/hash.hpp"
#include "Chunk.hpp"

#include "Blocktype.h"

namespace game
{
    using namespace mygl;

    class World
    {
        private:
            World();
            ~World() = default;

            World(const World&) = delete;
            World& operator=(const World&) = delete;
            World(World&&) = delete;
            World& operator=(World&&) = delete;

        public:
            static World& instance() {
                static World instance;
                return instance;
            }

        public:
            void render(const Camera3D &camera, const int &width, const int &height);
            uint8_t getBlockAt(int xpos, int ypos, int zpos);

            std::shared_mutex chunk_mtx;
            std::unordered_map<glm::ivec3, Chunk*> chunks;

        private:
            void renderTerrain(const Shader &shader, const ICamera &cam);
            void renderShadowMap(const Camera3D &camera, const int &width, const int &height);

            void createDepthQuadTexture();
            glm::mat4 computeLightSpaceMatrix(Camera3D &camera);

        private:
            // Camera3D &cam;

            Shader cube_shader;
            Shader cube_shadow;
            Shader depth_shader;
            Shader quad_depth_shader;

            GLuint block_textures;

            //shadowmap
            unsigned int depthMapFBO;
            unsigned int depthMap;
            const unsigned int shadow_width = 4096, shadow_height = 4096;
            float near_plane = -40.0f;
            float far_plane = 40.0f;
            glm::vec3 lightDir = glm::normalize(glm::vec3(- 0.3, -1.0, 0.2));
            glm::mat4 lightProjection = glm::ortho(-64.0f, 64.0f, -64.0f, 64.0f, near_plane, far_plane);
            glm::mat4 lightView = glm::lookAt(-lightDir, glm::vec3( 0.0f, 0.0f,  0.0f), glm::vec3( 0.0f, 1.0f,  0.0f));
            glm::mat4 lightSpaceMatrix = lightProjection * lightView;

            std::vector<glm::vec4> frustrum_corners;

    };
}