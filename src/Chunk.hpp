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
            GLuint diffuse_texture;
        public:
            Chunk(glm::vec3 pos);
            void createChunkVertices(glm::vec3 pos);
            void render(Shader shader, const ICamera &camera);

        private:
            GLuint vao;
            GLuint vbo;
            GLuint texture;

            int size = 16;
            std::vector<float> chunk_vertices;
            std::vector<float> cube_vertices
            {
                -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                0.5f, -0.5f, -0.5f,     1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                0.5f, 0.5f, -0.5f,      1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
                0.5f, 0.5f, -0.5f,      1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
                -0.5f, 0.5f, -0.5f,     0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
                -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f, 0.0f, -1.0f,

                -0.5f, -0.5f, 0.5f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f,     1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.5f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                0.5f, 0.5f, 0.5f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                -0.5f, 0.5f, 0.5f,     0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,

                -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    -1.0f, 0.0f,  0.0f,

                0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f, 0.0f,  0.0f,
                0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    1.0f, 0.0f,  0.0f,
                0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f, 0.0f,  0.0f,
                0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f, 0.0f,  0.0f,
                0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    1.0f, 0.0f,  0.0f,
                0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f, 0.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
                0.5f, -0.5f, -0.5f,     1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,     1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,     1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,

                -0.5f, 0.5f, -0.5f,    0.0f, 1.0f,    0.0f, 1.0f,  0.0f,
                0.5f, 0.5f, -0.5f,     1.0f, 1.0f,    0.0f, 1.0f,  0.0f,
                0.5f, 0.5f, 0.5f,      1.0f, 0.0f,    0.0f, 1.0f,  0.0f,
                0.5f, 0.5f, 0.5f,      1.0f, 0.0f,    0.0f, 1.0f,  0.0f,
                -0.5f, 0.5f, 0.5f,     0.0f, 0.0f,    0.0f, 1.0f,  0.0f,
                -0.5f, 0.5f, -0.5f,    0.0f, 1.0f,    0.0f, 1.0f,  0.0f
            };
    };
}