#include "Texture.hpp"

#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"

namespace mygl
{
    void loadTexture(const char *file, unsigned int &texture, int min_filter, int mag_filter, int wrap)
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

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap); //second argument was GL_REPEAT
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap); //second argument was GL_REPEAT
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << file << std::endl;
            stbi_image_free(data);
        }
    }
}