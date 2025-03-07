#pragma once

#include <glm/glm.hpp>
#include "glm/gtx/hash.hpp"
#include <vector>
#include <unordered_map>
#include <array>

#include "Blocktype.h"

namespace game
{
    class ChunkVertices
    {
        public:
            ChunkVertices();
            void createChunkVertices(const glm::ivec3 &worldpos);
            std::vector<uint32_t> chunk_vertices;
        private:
            void loadFaceVertices(const uint8_t *face_vertices, FaceOrientation orientation, const glm::ivec3 &local_pos, const glm::ivec3 &world_pos, uint8_t bt, std::vector<uint32_t> &vertices);
            uint8_t findBlockTextures(BlockType type, FaceOrientation orientation);
    };
}