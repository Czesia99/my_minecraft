#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ChunkMesh.hpp"

namespace game
{
    struct Chunk
    {
        std::vector<uint8_t>blocktypes;
        glm::ivec3 chunk_worldpos;
        ChunkMesh *mesh;
    };
}