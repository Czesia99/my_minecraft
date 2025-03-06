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
#include "Blocktype.h"

namespace game
{
    using namespace mygl;

    class ChunkMesh
    {
        public:
            std::vector<uint32_t> packed_vertices;

            // glm::ivec3 neighbor_chunkpos[6] =
            // {
            //     {0, 0, -1},
            //     {0, 0, 1},
            //     {-1, 0, 0},
            //     {1, 0, 0},
            //     {0, 1, 0},
            //     {0, -1, 0},
            // };
        public:
            ChunkMesh(const glm::ivec3 &pos);
            // ~Chunk() = default;
            void createChunkVertices();
            void createChunkMesh();
            void render(const Shader &shader, const ICamera &camera);
            void deleteChunk();
            int positionToIndex(glm::ivec3 pos);

        private:
            void loadFaceVertices(const std::vector<uint8_t> &vertices, FaceOrientation orientation, const glm::ivec3 &local_pos, const glm::ivec3 &world_pos, uint8_t bt);
            uint32_t packVerticesData();
            uint8_t findBlockTextures(BlockType type, FaceOrientation orientation);
            //uint8_t getBlockAt(int x, int y, int z, std::unordered_map<glm::ivec3, Chunk> nchunks);

        private:
            GLuint vao = 0;
            GLuint vbo = 0;

            glm::ivec3 worldpos;

            int size = 16;
            int vertex_count = 0;

            std::thread thread_chunk_vertices;

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