#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include "Shader.h"
#include "icamera.hpp"
#include "stb_image.h"

namespace mygl
{
    class Skybox {
        public:
            Skybox() {
                glGenVertexArrays(1, &vao);
                glBindVertexArray(vao);
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, skybox_vertices.size() * sizeof(float), skybox_vertices.data(), GL_STATIC_DRAW);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                std::vector<std::string> faces = {
                    "../textures/skybox/right.jpg",
                    "../textures/skybox/left.jpg",
                    "../textures/skybox/top.jpg",
                    "../textures/skybox/bottom.jpg",
                    "../textures/skybox/front.jpg",
                    "../textures/skybox/back.jpg"
                };
                cubemap_texture = load_cubemap(faces);
                skybox_shader = Shader("skybox.vs", "skybox.fs");
                transform.position = {0.0f, 0.0f, 0.0f};
            }

            void render(const ICamera &camera) {
                glm::mat4 projection = camera.get_projection_matrix();
                glm::mat4 view = glm::mat4(glm::mat3(camera.get_view_matrix()));
                // glDepthMask(GL_FALSE);
                glDepthFunc(GL_LEQUAL);
                skybox_shader.use();
                skybox_shader.setMat4("view", view);
                skybox_shader.setMat4("projection", projection);
                glBindVertexArray(vao);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glDepthFunc(GL_LESS);
                // glDepthMask(GL_TRUE);
            }
        public:
            Transform transform;

        private:
            unsigned int load_cubemap(std::vector<std::string> faces) 
            {
                unsigned int texture_id;
                glGenTextures(1, &texture_id);
                glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

                int width, height, nrChannels;
                for (unsigned int i = 0; i < faces.size(); i++)
                {
                    unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
                    if (data)
                    {
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                        );
                        stbi_image_free(data);
                    }
                    else
                    {
                        std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                        stbi_image_free(data);
                    }
                }
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

                return texture_id;
            }

        private:
            unsigned int vao;
            unsigned int vbo;
            unsigned int cubemap_texture;
            Shader skybox_shader;
            std::vector<float> skybox_vertices {
                // positions          
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f
            };
    };
}