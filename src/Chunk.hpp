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
            void loadFrontFaceVertices(glm::ivec3 &local_pos, glm::ivec3 &world_pos, int &index);
            void loadBackFaceVertices(glm::ivec3 &local_pos, glm::ivec3 &world_pos, int &index);
            void loadLeftFaceVertices(glm::ivec3 &local_pos, glm::ivec3 &world_pos, int &index);
            void loadRightFaceVertices(glm::ivec3 &local_pos, glm::ivec3 &world_pos, int &index);
            void loadTopFaceVertices(glm::ivec3 &local_pos, glm::ivec3 &world_pos, int &index);
            void loadBottomFaceVertices(glm::ivec3 &local_pos, glm::ivec3 &world_pos, int &index);

        private:
            GLuint vao;
            GLuint vbo;

            int size = 16;
            int vertex_count;
            glm::ivec3 chunk_pos;
            std::vector<uint8_t>blocktypes;
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

            std::vector<float> front_face_vertices
            {
                0.0f, 0.0f, 0.0f,    1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                1.0f, 1.0f, 0.0f,      0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
                1.0f, 0.0f, 0.0f,     0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                1.0f, 1.0f, 0.0f,      0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, 0.0f,    1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
                0.0f, 1.0f, 0.0f,     1.0f, 1.0f,    0.0f, 0.0f, -1.0f,
            };

            std::vector<float> back_face_vertices
            {
                0.0f, 0.0f, 1.0f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f,     1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f,      1.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                0.0f, 1.0f, 1.0f,     0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
                0.0f, 0.0f, 1.0f,    0.0f, 0.0f,    0.0f, 0.0f, 1.0f, 
            };

            std::vector<float> left_face_vertices
            {
                0.0f,  1.0f,  1.0f,    1.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
                0.0f,  1.0f, 0.0f,    0.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
                0.0f, 0.0f, 0.0f,    0.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
                0.0f, 0.0f, 0.0f,    0.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
                0.0f, 0.0f,  1.0f,    1.0f, 0.0f,    -1.0f, 0.0f,  0.0f,
                0.0f,  1.0f,  1.0f,    1.0f, 1.0f,    -1.0f, 0.0f,  0.0f,
            };

            std::vector<float> right_face_vertices
            {
                1.0f,  1.0f,  1.0f,    0.0f, 1.0f,    1.0f, 0.0f,  0.0f,
                1.0f, 0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 0.0f,  0.0f,
                1.0f,  1.0f, 0.0f,    1.0f, 1.0f,    1.0f, 0.0f,  0.0f,
                1.0f, 0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 0.0f,  0.0f,
                1.0f,  1.0f,  1.0f,    0.0f, 1.0f,    1.0f, 0.0f,  0.0f,
                1.0f, 0.0f,  1.0f,    0.0f, 0.0f,    1.0f, 0.0f,  0.0f, 
            };

            std::vector<float> bottom_face_vertices 
            {
                0.0f, 0.0f, 0.0f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
                1.0f, 0.0f, 0.0f,     0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
                1.0f, 0.0f,  1.0f,     0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                1.0f, 0.0f,  1.0f,     0.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                0.0f, 0.0f,  1.0f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
                0.0f, 0.0f, 0.0f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
            };

            std::vector<float> top_face_vertices
            {
                0.0f, 1.0f, 0.0f,    1.0f, 0.0f,    0.0f, 1.0f,  0.0f,
                1.0f, 1.0f, 1.0f,      0.0f, 1.0f,    0.0f, 1.0f,  0.0f,
                1.0f, 1.0f, 0.0f,     0.0f, 0.0f,    0.0f, 1.0f,  0.0f,
                0.0f, 1.0f, 1.0f,     1.0f, 1.0f,    0.0f, 1.0f,  0.0f,
                1.0f, 1.0f, 1.0f,      0.0f, 1.0f,    0.0f, 1.0f,  0.0f,
                0.0f, 1.0f, 0.0f,    1.0f, 0.0f,    0.0f, 1.0f,  0.0f
            };
    };
}