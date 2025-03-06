#include "ChunkMesh.hpp"
#include "World.hpp"
#include "Chunk.hpp"

namespace game
{
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

    ChunkMesh::ChunkMesh(const glm::ivec3 &pos) : worldpos(pos) {}

    void ChunkMesh::createChunkMesh()
    {
        if (vao == 0) {
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(uint32_t), (void*)0);
            glEnableVertexAttribArray(0);

            // glBindVertexArray(0);
        }
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, packed_vertices.size() * sizeof(uint32_t), packed_vertices.data(), GL_STATIC_DRAW);
        // packed_vertices.shrink_to_fit();
    }

    void ChunkMesh::createChunkVertices()
    {
        vertex_count = 0;
        packed_vertices.clear();

        const glm::ivec3 neighbor_chunkpos[7] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}, {0, 0, 0}};
        std::unordered_map<glm::ivec3, Chunk> neighbor_chunks;
        //lock mtx
        World::instance().chunk_mtx.lock();
        for (int i = 0; i < 7; i++)
        {
            auto it = World::instance().chunks.find(worldpos + neighbor_chunkpos[i] * 16);
            if (it != World::instance().chunks.end())
                neighbor_chunks[worldpos + neighbor_chunkpos[i] * 16] = it->second;
        }
        World::instance().chunk_mtx.unlock();

        for (int z = 0; z < size; z++) {
        for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {

            int index = x + y*16 + z*16*16;

            bool display = false;

            glm::ivec3 local_pos = {x, y, z};
            glm::ivec3 world_pos = local_pos + worldpos;
            uint8_t blocktype = getBlockAt(world_pos.x, world_pos.y, world_pos.z, neighbor_chunks);

            if (blocktype == 0) continue;

            if (getBlockAt(world_pos.x, world_pos.y, world_pos.z - 1, neighbor_chunks) == 0)
            {
                loadFaceVertices(front_face_vertices, FaceOrientation::Front, local_pos, world_pos, blocktype);
            }
            if (getBlockAt(world_pos.x, world_pos.y, world_pos.z + 1, neighbor_chunks) == 0)
            {
                loadFaceVertices(back_face_vertices, FaceOrientation::Back, local_pos, world_pos, blocktype);
            }
            if (getBlockAt(world_pos.x - 1, world_pos.y, world_pos.z, neighbor_chunks) == 0)
            {
                loadFaceVertices(left_face_vertices, FaceOrientation::Left, local_pos, world_pos, blocktype);
            }
            if (getBlockAt(world_pos.x + 1, world_pos.y, world_pos.z, neighbor_chunks) == 0)
            {
                loadFaceVertices(right_face_vertices, FaceOrientation::Right, local_pos, world_pos, blocktype);
            }
            if (getBlockAt(world_pos.x, world_pos.y + 1, world_pos.z, neighbor_chunks) == 0)
            {
                loadFaceVertices(top_face_vertices, FaceOrientation::Top, local_pos, world_pos, blocktype);
            }
            if (getBlockAt(world_pos.x, world_pos.y - 1, world_pos.z, neighbor_chunks) == 0)
            {
                loadFaceVertices(bottom_face_vertices, FaceOrientation::Bottom, local_pos, world_pos, blocktype);
            }
        }}}
    }

    void ChunkMesh::render(const Shader &shader, const ICamera &camera)
    {
        if (vertex_count == 0)
            return;

        shader.setVec3i("chunkpos", worldpos);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    }

    void ChunkMesh::loadFaceVertices(const std::vector<uint8_t> &vertices, FaceOrientation orientation, const glm::ivec3 &local_pos, const glm::ivec3 &world_pos, uint8_t bt)
    {
        const glm::ivec3 direction = getFaceOrientationVector(orientation);
        const glm::ivec3 ndir = direction + local_pos;
        int neighbor = World::instance().positionToIndex(ndir);

        for (int i = 0; i < vertices.size(); i+= 6)
        {
            uint32_t packed;
            uint8_t block_texture = findBlockTextures(getBlockType(bt), orientation);
            packed = ((vertices[i] + local_pos.x) & 0b11111) << 0       |
                        ((vertices[i + 1] + local_pos.y) & 0b11111) << 5   |
                        ((vertices[i + 2] + local_pos.z) & 0b11111) << 10  |
                        ((vertices[i + 3]) & 0b1) << 15                    |
                        ((vertices[i + 4]) & 0b1) << 16                    |
                        ((vertices[i + 5]) & 0b111) << 17                  |
                        (block_texture & 0b11111) << 20;

            packed_vertices.push_back(packed);
            vertex_count += 1;
        }
    }

    uint8_t ChunkMesh::findBlockTextures(BlockType type, FaceOrientation orientation)
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

    void ChunkMesh::deleteChunk()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}