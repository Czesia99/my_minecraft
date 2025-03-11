#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "glm/gtx/hash.hpp"
#include <vector>
#include <thread>
#include <unordered_map>
#include <array>

#include "MYGL/Shader.h"
#include "MYGL/Transform.hpp"
#include "MYGL/ICamera.hpp"

namespace game
{
    using namespace mygl;

    class ChunkMesh
    {
        public:
            ChunkMesh(const glm::ivec3 &pos);
            // ~Chunk() = default;
            void createChunkMesh(const std::vector<uint32_t> &vertices);
            void render(const Shader &shader, const ICamera &camera);
            // int positionToIndex(glm::ivec3 pos);
            void deleteChunk();

        private:
            GLuint vao = 0;
            GLuint vbo = 0;

            glm::ivec3 worldpos;
            size_t vertex_count;
    };
}