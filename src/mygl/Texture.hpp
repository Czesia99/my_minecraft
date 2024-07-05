#pragma once

#include <glad/glad.h>

namespace mygl 
{
    void loadTexture(const char *file, unsigned int &texture, int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR, int wrap = GL_REPEAT);
}