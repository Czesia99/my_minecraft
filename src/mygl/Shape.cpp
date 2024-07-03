#include "Shape.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace mygl {
    //3D PRIMITIVES
    Cube::Cube()
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    void Cube::render(Shader shader, const ICamera &camera)
    {
        shader.use();
        if (diffuse_texture) {
            shader.setInt("material.diffuse", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuse_texture);
        }

        if (specular_texture) {
            shader.setInt("material.specular", 1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specular_texture);
        }

        glm::mat4 model = transform.getModelMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();
        glm::mat3 normal = glm::transpose(glm::inverse(model));

        shader.setMat4("model", model); 
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat3("normal", normal);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void Cube::add_texture(const char *file, unsigned int &texture)
    {
        glGenTextures(1, &texture);
        
        int width, height, nrComponents;
        unsigned char *data = stbi_load(file, &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << file << std::endl;
            stbi_image_free(data);
        }
    }
}