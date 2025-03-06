#pragma once

#include <glm/glm.hpp>
#include <array>

namespace game
{
    struct Chunk
    {
        std::array<uint8_t, 4096>blocktypes;
        glm::ivec3 worldpos;
    };
}