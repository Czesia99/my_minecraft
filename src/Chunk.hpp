#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "mygl/Shader.h"
#include "mygl/Transform.hpp"
#include "mygl/ICamera.hpp"

namespace game
{
    using namespace mygl;

    class Chunk
    {
        public:
            Transform transform;
            // GLuint diffuse_texture;
        public:
            Chunk(glm::ivec3 pos, std::vector<uint8_t>&blocktypes);
            // ~Chunk() = default;
            void createChunkVertices(glm::ivec3 pos);
            void render(const Shader &shader, const ICamera &camera);
            void deleteChunk();

        private:
            GLuint vao;
            GLuint vbo;
            // GLuint texture;

            int size = 16;
            int vertex_count;
            glm::ivec3 chunk_pos;
            std::vector<uint8_t>blocktypes;
            // int blocks[4096];
            std::vector<float> chunk_vertices;
            std::vector<float> cube_vertices
            {
                //front face -Z
                0.0f, 0.0f, 0.0f,    1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                1.0f, 1.0f, 0.0f,      0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
                1.0f, 0.0f, 0.0f,     0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                1.0f, 1.0f, 0.0f,      0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, 0.0f,    1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                0.0f, 1.0f, 0.0f,     1.0f, 1.0f,    0.0f, 0.0f, -1.0f,

                //back face +Z
                0.0f, 0.0f, 1.0f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f,     1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                0.0f, 1.0f, 1.0f,     0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,

                //left face -X
                0.0f,  1.0f,  1.0f,    1.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
                0.0f,  1.0f, 0.0f,    0.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
                0.0f, 0.0f, 0.0f,    0.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
                0.0f, 0.0f, 0.0f,    0.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
                0.0f, 0.0f,  1.0f,    1.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
                0.0f,  1.0f,  1.0f,    1.0f, 1.0f,    -1.0f, 0.0f,  0.0f,

                //right face +X
                1.0f,  1.0f,  1.0f,    0.0f, 1.0f,    1.0f, 0.0f,  0.0f,
                1.0f, 0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 0.0f,  0.0f,
                1.0f,  1.0f, 0.0f,    1.0f, 1.0f,    1.0f, 0.0f,  0.0f,
                1.0f, 0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 0.0f,  0.0f,
                1.0f,  1.0f,  1.0f,    0.0f, 1.0f,    1.0f, 0.0f,  0.0f,
                1.0f, 0.0f,  1.0f,    0.0f, 0.0f,    1.0f, 0.0f,  0.0f,

                // bottom face -Y
                0.0f, 0.0f, 0.0f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
                1.0f, 0.0f, 0.0f,     0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
                1.0f, 0.0f,  1.0f,     0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                1.0f, 0.0f,  1.0f,     0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                0.0f, 0.0f,  1.0f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                0.0f, 0.0f, 0.0f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f,

                //top face +Y
                0.0f, 1.0f, 0.0f,    1.0f, 0.0f,    0.0f, 1.0f,  0.0f,
                1.0f, 1.0f, 1.0f,      0.0f, 1.0f,    0.0f, 1.0f,  0.0f,
                1.0f, 1.0f, 0.0f,     0.0f, 0.0f,    0.0f, 1.0f,  0.0f,
                0.0f, 1.0f, 1.0f,     1.0f, 1.0f,    0.0f, 1.0f,  0.0f,
                1.0f, 1.0f, 1.0f,      0.0f, 1.0f,    0.0f, 1.0f,  0.0f,
                0.0f, 1.0f, 0.0f,    1.0f, 0.0f,    0.0f, 1.0f,  0.0f
            };
    };
}