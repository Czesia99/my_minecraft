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
            std::vector<uint32_t> createChunkVertices(glm::ivec3 worldpos);
            std::vector<uint32_t> chunk_vertices;
        private:
            void loadFaceVertices(const std::vector<uint8_t> &face_vertices, FaceOrientation orientation, const glm::ivec3 &local_pos, const glm::ivec3 &world_pos, uint8_t bt, std::vector<uint32_t> &vertices);
            uint8_t findBlockTextures(BlockType type, FaceOrientation orientation);
            public:
            std::vector<uint8_t> front_face_vertices {
                //front face -Z
                //normal : 0.0, 0.0, -1.0
                0, 0, 0,    1, 0,   0,
                1, 1, 0,    0, 1,   0,
                1, 0, 0,    0, 0,   0,
                1, 1, 0,    0, 1,   0,
                0, 0, 0,    1, 0,   0,
                0, 1, 0,    1, 1,   0,
            };
            std::vector<uint8_t> back_face_vertices {
                //back face +Z
                //normal : 0.0, 0.0, 1.0
                0, 0, 1,    0, 0,    1,
                1, 0, 1,    1, 0,    1,
                1, 1, 1,    1, 1,    1,
                1, 1, 1,    1, 1,    1,
                0, 1, 1,    0, 1,    1,
                0, 0, 1,    0, 0,    1,
            };
            std::vector<uint8_t> left_face_vertices {
                //left face -X
                //normal : -1.0, 0.0, 0.0
                0, 1, 1,    1, 1,    2,
                0, 1, 0,    0, 1,    2,
                0, 0, 0,    0, 0,    2,
                0, 0, 0,    0, 0,    2,
                0, 0, 1,    1, 0,    2,
                0, 1, 1,    1, 1,    2,
            };
            std::vector<uint8_t> right_face_vertices {
                //right face +X
                //normal: 1.0, 0.0, 0.0
                1, 1, 1,    0, 1,    3,
                1, 0, 0,    1, 0,    3,
                1, 1, 0,    1, 1,    3,
                1, 0, 0,    1, 0,    3,
                1, 1, 1,    0, 1,    3,
                1, 0, 1,    0, 0,    3,
            };
            std::vector<uint8_t> bottom_face_vertices {
                // bottom face -Y
                //normal: 0.0, -1.0, 0.0
                0, 0, 0,    1, 1,    4,
                1, 0, 0,    0, 1,    4,
                1, 0, 1,    0, 0,    4,
                1, 0, 1,    0, 0,    4,
                0, 0, 1,    1, 0,    4,
                0, 0, 0,    1, 1,    4,
            };
            std::vector<uint8_t> top_face_vertices {
                //top face +Y
                //normal: 0.0, 1.0, 0.0
                0, 1, 0,    1, 0,    5,
                1, 1, 1,    0, 1,    5,
                1, 1, 0,    0, 0,    5,
                0, 1, 1,    1, 1,    5,
                1, 1, 1,    0, 1,    5,
                0, 1, 0,    1, 0,    5,
            };
    };
}