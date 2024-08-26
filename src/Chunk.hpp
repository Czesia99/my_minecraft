#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <thread>

#include "MYGL/Shader.h"
#include "MYGL/Transform.hpp"
#include "MYGL/ICamera.hpp"

namespace game
{
    using namespace mygl;

    class Chunk
    {
        public:
            Transform transform;
            std::vector<float> chunk_vertices;
            std::vector<uint8_t>blocktypes;
            glm::ivec3 chunk_pos;
            // GLuint diffuse_texture;
        public:
            Chunk(glm::ivec3 pos, std::vector<uint8_t>&blocktypes);
            // ~Chunk() = default;
            void createChunkVertices();
            void render(const Shader &shader, const ICamera &camera);
            void deleteChunk();
            int positionToIndex(glm::ivec3 pos);

        private:
            void loadFaceVertices(std::vector<float> vertices, glm::ivec3 direction, glm::ivec3 &local_pos, glm::ivec3 &world_pos, int index);

        private:
            GLuint vao;
            GLuint vbo;

            int size = 16;
            int vertex_count;
            
            
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