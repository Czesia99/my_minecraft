#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Transform.hpp"
#include "ICamera.hpp"

namespace mygl {
    //2D PRIMITIVES
    class Triangle {
        public:
            Triangle() {
                glGenVertexArrays(1, &vao);
                glBindVertexArray(vao);
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, triangle_vertices.size() * sizeof(float), triangle_vertices.data(), GL_STATIC_DRAW);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
                glEnableVertexAttribArray(1);
            }

            void render(Shader shader, const ICamera &camera)
            {
                shader.use();
                glm::mat4 mat = transform.getModelMatrix();
                glm::mat4 projection = camera.getProjectionMatrix();
                glm::mat4 view = camera.getViewMatrix();

                shader.setMat4("model", mat);
                shader.setMat4("projection", projection);
                shader.setMat4("view", view);

                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            }

        public:
            Transform transform;

        private:
            unsigned int vao;
            unsigned int vbo;
            //triangle
            std::vector<float> triangle_vertices = {
                0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
                -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            };
    };

    class MyRectangle {
        public:
            MyRectangle() {
                glGenVertexArrays(1, &vao);
                glBindVertexArray(vao);
                glGenBuffers(1, &ebo);
                glGenBuffers(1, &vbo);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);

                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
                glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_vertices), rectangle_vertices, GL_STATIC_DRAW);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
                glEnableVertexAttribArray(1);
            }

            void render(Shader shader, const ICamera &camera)
            {
                shader.use();
                glm::mat4 mat = transform.getModelMatrix();
                glm::mat4 projection = camera.getProjectionMatrix();
                glm::mat4 view = camera.getViewMatrix();

                shader.setMat4("model", mat);
                shader.setMat4("projection", projection);
                shader.setMat4("view", view);

                glBindVertexArray(vao);
                // glDrawArrays(GL_TRIANGLES, 0, 3);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }

        public:
            Transform transform;
            unsigned int texture;

        private:
            unsigned int ebo;
            unsigned int vao;
            unsigned int vbo;

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
            void add_texture(const char *file, unsigned int &texture);

        public:
            Transform transform;
            unsigned int diffuse_texture;
            unsigned int specular_texture;
        private:
            unsigned int vao;
            unsigned int vbo;
            unsigned int texture;
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

#endif