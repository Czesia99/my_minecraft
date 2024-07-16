#pragma once

#include <glad/glad.h>
#include <vector>
#include <string>

namespace mygl 
{
    void loadTexture(const char *file, unsigned int &texture, int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR, int wrap = GL_REPEAT);
    void loadTextureArray(const std::vector<std::string>files, unsigned int &texture, int width, int height, int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR, int wrap = GL_REPEAT);
}