#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
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

    class Chunk
    {
        public:
            Transform transform;
            std::vector<float> chunk_vertices;
            std::vector<uint8_t>blocktypes;
            glm::ivec3 chunk_worldpos;
            // GLuint diffuse_texture;
        public:
            Chunk(const glm::ivec3 &pos, const std::vector<uint8_t> &blocktypes);
            // ~Chunk() = default;
            void createChunkVertices();
            void createChunkMesh();
            void render(const Shader &shader, const ICamera &camera);
            void deleteChunk();
            int positionToIndex(glm::ivec3 pos);

        private:
            void loadFaceVertices(std::vector<float> &vertices, FaceOrientation orientation, const glm::ivec3 &local_pos, const glm::ivec3 &world_pos, int index);
            int findBlockTextures(BlockType type, FaceOrientation orientation);

        private:
            GLuint vao = 0;
            GLuint vbo = 0;

            int size = 16;
            int vertex_count = 0;

            std::thread thread_chunk_vertices;

            std::vector<float> front_face_vertices {
                //front face -Z
                0.0f, 0.0f, 0.0f,    1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                1.0f, 1.0f, 0.0f,      0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
                1.0f, 0.0f, 0.0f,     0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                1.0f, 1.0f, 0.0f,      0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, 0.0f,    1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                0.0f, 1.0f, 0.0f,     1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
            };
            std::vector<float> back_face_vertices {
                //back face +Z
                0.0f, 0.0f, 1.0f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f,     1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                0.0f, 1.0f, 1.0f,     0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
            };
            std::vector<float> left_face_vertices {
                //left face -X
                0.0f,  1.0f,  1.0f,    1.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
                0.0f,  1.0f, 0.0f,    0.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
                0.0f, 0.0f, 0.0f,    0.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
                0.0f, 0.0f, 0.0f,    0.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
                0.0f, 0.0f,  1.0f,    1.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
                0.0f,  1.0f,  1.0f,    1.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
            };
            std::vector<float> right_face_vertices {
                //right face +X
                1.0f,  1.0f,  1.0f,    0.0f, 1.0f,    1.0f, 0.0f,  0.0f,
                1.0f, 0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 0.0f,  0.0f,
                1.0f,  1.0f, 0.0f,    1.0f, 1.0f,    1.0f, 0.0f,  0.0f,
                1.0f, 0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 0.0f,  0.0f,
                1.0f,  1.0f,  1.0f,    0.0f, 1.0f,    1.0f, 0.0f,  0.0f,
                1.0f, 0.0f,  1.0f,    0.0f, 0.0f,    1.0f, 0.0f,  0.0f,
            };
            std::vector<float> bottom_face_vertices {
                // bottom face -Y
                0.0f, 0.0f, 0.0f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
                1.0f, 0.0f, 0.0f,     0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
                1.0f, 0.0f,  1.0f,     0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                1.0f, 0.0f,  1.0f,     0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                0.0f, 0.0f,  1.0f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                0.0f, 0.0f, 0.0f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
            };
            std::vector<float> top_face_vertices {
                //top face +Y
                0.0f, 1.0f, 0.0f,    1.0f, 0.0f,    0.0f, 1.0f,  0.0f,
                1.0f, 1.0f, 1.0f,      0.0f, 1.0f,    0.0f, 1.0f,  0.0f,
                1.0f, 1.0f, 0.0f,     0.0f, 0.0f,    0.0f, 1.0f,  0.0f,
                0.0f, 1.0f, 1.0f,     1.0f, 1.0f,    0.0f, 1.0f,  0.0f,
                1.0f, 1.0f, 1.0f,      0.0f, 1.0f,    0.0f, 1.0f,  0.0f,
                0.0f, 1.0f, 0.0f,    1.0f, 0.0f,    0.0f, 1.0f,  0.0f,
            };
    };
}