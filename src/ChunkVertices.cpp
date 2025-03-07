#include "ChunkVertices.hpp"
#include "World.hpp"

namespace game
{
    const uint8_t front_face_vertices[] {
        //front face -Z
        //normal : 0.0, 0.0, -1.0
        0, 0, 0,    1, 0,   0,
        1, 1, 0,    0, 1,   0,
        1, 0, 0,    0, 0,   0,
        1, 1, 0,    0, 1,   0,
        0, 0, 0,    1, 0,   0,
        0, 1, 0,    1, 1,   0,
    };
    const uint8_t back_face_vertices[] {
        //back face +Z
        //normal : 0.0, 0.0, 1.0
        0, 0, 1,    0, 0,    1,
        1, 0, 1,    1, 0,    1,
        1, 1, 1,    1, 1,    1,
        1, 1, 1,    1, 1,    1,
        0, 1, 1,    0, 1,    1,
        0, 0, 1,    0, 0,    1,
    };
    const uint8_t left_face_vertices[] {
        //left face -X
        //normal : -1.0, 0.0, 0.0
        0, 1, 1,    1, 1,    2,
        0, 1, 0,    0, 1,    2,
        0, 0, 0,    0, 0,    2,
        0, 0, 0,    0, 0,    2,
        0, 0, 1,    1, 0,    2,
        0, 1, 1,    1, 1,    2,
    };
    const uint8_t right_face_vertices[] {
        //right face +X
        //normal: 1.0, 0.0, 0.0
        1, 1, 1,    0, 1,    3,
        1, 0, 0,    1, 0,    3,
        1, 1, 0,    1, 1,    3,
        1, 0, 0,    1, 0,    3,
        1, 1, 1,    0, 1,    3,
        1, 0, 1,    0, 0,    3,
    };
    const uint8_t bottom_face_vertices[] {
        // bottom face -Y
        //normal: 0.0, -1.0, 0.0
        0, 0, 0,    1, 1,    4,
        1, 0, 0,    0, 1,    4,
        1, 0, 1,    0, 0,    4,
        1, 0, 1,    0, 0,    4,
        0, 0, 1,    1, 0,    4,
        0, 0, 0,    1, 1,    4,
    };
    const uint8_t top_face_vertices[] {
        //top face +Y
        //normal: 0.0, 1.0, 0.0
        0, 1, 0,    1, 0,    5,
        1, 1, 1,    0, 1,    5,
        1, 1, 0,    0, 0,    5,
        0, 1, 1,    1, 1,    5,
        1, 1, 1,    0, 1,    5,
        0, 1, 0,    1, 0,    5,
    };

    uint8_t getBlockAt(int x, int y, int z, std::unordered_map<glm::ivec3, Chunk> &nchunks)
    {
        glm::ivec3 chunk_pos = glm::floor(glm::vec3(x,y,z) / 16.0f) * 16.0f;
        glm::ivec3 local_pos = {x % 16, y % 16, z % 16};
        if (local_pos.x < 0) local_pos.x += 16;
        if (local_pos.y < 0) local_pos.y += 16;
        if (local_pos.z < 0) local_pos.z += 16;

        glm::ivec3 cube_pos = {x, y, z};

        auto it =  nchunks.find(chunk_pos);
        if (it != nchunks.end())
        {
            uint8_t blocktype = it->second.blocktypes[World::instance().positionToIndex(local_pos)];
            return blocktype;
        } else
            return 0;
    }

    ChunkVertices::ChunkVertices() {};

    void ChunkVertices::createChunkVertices(glm::ivec3 worldpos)
    {
        const glm::ivec3 neighbor_chunkpos[7] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}, {0, 0, 0}};
        std::unordered_map<glm::ivec3, Chunk> neighbor_chunks;
        //lock mtx
        World::instance().chunk_mtx.lock_shared();
        for (int i = 0; i < 7; i++)
        {
            auto it = World::instance().chunks.find(worldpos + neighbor_chunkpos[i] * 16);
            if (it != World::instance().chunks.end())
                neighbor_chunks[worldpos + neighbor_chunkpos[i] * 16] = it->second;
        }
        World::instance().chunk_mtx.unlock_shared();

        for (int z = 0; z < 16; z++) {
        for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {

            int index = x + y*16 + z*16*16;

            bool display = false;

            glm::ivec3 local_pos = {x, y, z};
            glm::ivec3 world_pos = local_pos + worldpos;
            uint8_t blocktype = getBlockAt(world_pos.x, world_pos.y, world_pos.z, neighbor_chunks);

            if (blocktype == 0) continue;

            if (getBlockAt(world_pos.x, world_pos.y, world_pos.z - 1, neighbor_chunks) == 0)
            {
                loadFaceVertices(front_face_vertices, FaceOrientation::Front, local_pos, world_pos, blocktype, chunk_vertices);
            }
            if (getBlockAt(world_pos.x, world_pos.y, world_pos.z + 1, neighbor_chunks) == 0)
            {
                loadFaceVertices(back_face_vertices, FaceOrientation::Back, local_pos, world_pos, blocktype, chunk_vertices);
            }
            if (getBlockAt(world_pos.x - 1, world_pos.y, world_pos.z, neighbor_chunks) == 0)
            {
                loadFaceVertices(left_face_vertices, FaceOrientation::Left, local_pos, world_pos, blocktype, chunk_vertices);
            }
            if (getBlockAt(world_pos.x + 1, world_pos.y, world_pos.z, neighbor_chunks) == 0)
            {
                loadFaceVertices(right_face_vertices, FaceOrientation::Right, local_pos, world_pos, blocktype, chunk_vertices);
            }
            if (getBlockAt(world_pos.x, world_pos.y + 1, world_pos.z, neighbor_chunks) == 0)
            {
                loadFaceVertices(top_face_vertices, FaceOrientation::Top, local_pos, world_pos, blocktype, chunk_vertices);
            }
            if (getBlockAt(world_pos.x, world_pos.y - 1, world_pos.z, neighbor_chunks) == 0)
            {
                loadFaceVertices(bottom_face_vertices, FaceOrientation::Bottom, local_pos, world_pos, blocktype, chunk_vertices);
            }
        }}}
    }

    void ChunkVertices::loadFaceVertices(const uint8_t *face_vertices, FaceOrientation orientation, const glm::ivec3 &local_pos, const glm::ivec3 &world_pos, uint8_t bt, std::vector<uint32_t> &vertices)
    {
        const glm::ivec3 direction = getFaceOrientationVector(orientation);
        const glm::ivec3 ndir = direction + local_pos;
        int neighbor = World::instance().positionToIndex(ndir);

        for (int i = 0; i < 36; i+= 6)
        {
            uint32_t packed;
            uint8_t block_texture = findBlockTextures(getBlockType(bt), orientation);
            packed = ((face_vertices[i] + local_pos.x) & 0b11111) << 0       |
                        ((face_vertices[i + 1] + local_pos.y) & 0b11111) << 5   |
                        ((face_vertices[i + 2] + local_pos.z) & 0b11111) << 10  |
                        ((face_vertices[i + 3]) & 0b1) << 15                    |
                        ((face_vertices[i + 4]) & 0b1) << 16                    |
                        ((face_vertices[i + 5]) & 0b111) << 17                  |
                        (block_texture & 0b11111) << 20;

            vertices.push_back(packed);
        }
    }

    uint8_t ChunkVertices::findBlockTextures(BlockType type, FaceOrientation orientation)
    {
        if (orientation == FaceOrientation::Front || orientation == FaceOrientation::Back || orientation == FaceOrientation::Left || orientation == FaceOrientation::Right)
        {
            return textures_umap.at(type).at(1);
        }

        if (orientation == FaceOrientation::Top)
        {
            return textures_umap.at(type).at(0);
        }

        if (orientation == FaceOrientation::Bottom)
        {
            return textures_umap.at(type).at(2);
        }
        return 0;
    }
}