#pragma once

#include <glad/glad.h>
#include <vector>
#include "Shader.h"
#include "Transform.hpp"

namespace mygl 
{
    class ICamera;
    //2D PRIMITIVES
    class Triangle {
        public:
            Triangle();
            void render(Shader shader, const ICamera &camera);

        public:
            Transform transform;

        private:
            GLuint vao;
            GLuint vbo;
            //triangle
            std::vector<float> triangle_vertices = {
                0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            };
    };

    class MyRectangle {
        public:
            MyRectangle();
            void render(Shader shader, const ICamera &camera);

        public:
            Transform transform;
            GLuint texture;

        private:
            GLuint ebo;
            GLuint vao;
            GLuint vbo;

            float rectangle_vertices[20] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom left
                0.5f, -0.5f, 0.0f,  1.0f, 0.0f,// bottom right
                0.5f,  0.5f, 0.0f,  1.0f, 1.0f,// top right
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,// top left
            };

            unsigned int indices[6] = {  // note that we start from 0!
                0, 1, 2,   // first triangle
                0, 2, 3    // second triangle
            };
    };

    //3D PRIMITIVES
    class Cube {
        public:
            Cube();
            void render(Shader shader, const ICamera &camera);
            void setDiffuseTexture(GLuint &texture);
        public:
            Transform transform;
            GLuint diffuse_texture;
            GLuint specular_texture;
        private:
            GLuint vao;
            GLuint vbo;
            GLuint texture;
            //pos 3 //texture coords 2 // normal 3
            std::vector<float> vertices
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