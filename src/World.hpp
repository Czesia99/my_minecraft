#pragma once

#include "MYGL/Camera3D.hpp"
#include "MYGL/Texture.hpp"

#include "glm/gtx/hash.hpp"

#include <unordered_map>
#include <shared_mutex>

#include "Chunk.hpp"
#include "ChunkMesh.hpp"
#include "Shadowmap.hpp"

namespace game
{
    using namespace mygl;

    class World
    {
        private:
            World();
            ~World();

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
            void renderTerrain(const Shader &shader, const ICamera &cam);
            void createOrReplaceChunk(const glm::ivec3 &pos, const std::array<uint8_t, 4096> &blocktypes);
            uint8_t getBlockAt(int xpos, int ypos, int zpos);
            uint8_t getBlockAt(float xpos, float ypos, float zpos);
            int positionToIndex(glm::ivec3 pos);
            void clearAllChunks();


            std::shared_mutex chunk_mtx;
            std::unordered_map<glm::ivec3, Chunk> chunks;
            std::unordered_map<glm::ivec3, ChunkMesh> chunkMeshes;
            Shadowmap shadowmap;

            bool fog_display = false;
            float fog_maxdist = 600.0f;
            float fog_mindist = 150.0f;
            glm::vec4 fog_color = glm::vec4(0.9, 0.9, 0.9, 1.0);

        private:
            struct ChunkAABB
            {
                glm::vec3 min;
                glm::vec3 max;
            };

            GLuint chunk_vao = 0;
            Shader cube_shadow;
            GLuint block_textures;
            ChunkAABB getChunkAABB(const glm::ivec3 &pos);
            bool boxInFrustum(const glm::vec4 planes[6], ChunkAABB const &box);
    };
}