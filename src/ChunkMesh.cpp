#include "ChunkMesh.hpp"
#include "World.hpp"

namespace game
{
    ChunkMesh::ChunkMesh(const glm::ivec3 &pos, const std::vector<uint8_t>&blocktypes) : worldpos(pos), blocktypes(blocktypes) {}

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

        // for (int i = 0; i < 6; i++)
        // {
        //     auto it = World::instance().chunks.find(worldpos + neighbor_chunkpos[i] * 16);
        //     glm::ivec3 pos = neighbor_chunkpos[i];
        //     if (it != World::instance().chunks.end()) {
        //         std::vector<uint8_t> bt = it->second->blocktypes;
        //         // Chunk n = *(World::instance().chunks[worldpos + neighbor_chunkpos[i]]);
        //         neighbor_chunks[pos] = bt;
        //     } else {
        //         neighbor_chunks[pos] = std::vector<uint8_t>(4096, 0);
        //     }
        // }

        for (int z = 0; z < size; z++) {
        for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {

            int index = x + y*16 + z*16*16;

            if (blocktypes[index] == 0) continue;
            bool display = false;

            glm::ivec3 local_pos = {x, y, z};
            glm::ivec3 world_pos = local_pos + worldpos;

            if (World::instance().getBlockAt(world_pos.x, world_pos.y, world_pos.z - 1) == 0)
            {
                loadFaceVertices(front_face_vertices, FaceOrientation::Front, local_pos, world_pos, index);
            }
            if (World::instance().getBlockAt(world_pos.x, world_pos.y, world_pos.z + 1) == 0)
            {
                loadFaceVertices(back_face_vertices, FaceOrientation::Back, local_pos, world_pos, index);
            }
            if (World::instance().getBlockAt(world_pos.x - 1, world_pos.y, world_pos.z) == 0)
            {
                loadFaceVertices(left_face_vertices, FaceOrientation::Left, local_pos, world_pos, index);
            }
            if (World::instance().getBlockAt(world_pos.x + 1, world_pos.y, world_pos.z) == 0)
            {
                loadFaceVertices(right_face_vertices, FaceOrientation::Right, local_pos, world_pos, index);
            }
            if (World::instance().getBlockAt(world_pos.x, world_pos.y + 1, world_pos.z) == 0)
            {
                loadFaceVertices(top_face_vertices, FaceOrientation::Top, local_pos, world_pos, index);
            }
            if (World::instance().getBlockAt(world_pos.x, world_pos.y - 1, world_pos.z) == 0)
            {
                loadFaceVertices(bottom_face_vertices, FaceOrientation::Bottom, local_pos, world_pos, index);
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

    void ChunkMesh::loadFaceVertices(const std::vector<uint8_t> &vertices, FaceOrientation orientation, const glm::ivec3 &local_pos, const glm::ivec3 &world_pos, int index)
    {
        const glm::ivec3 direction = getFaceOrientationVector(orientation);
        const glm::ivec3 ndir = direction + local_pos;
        int neighbor = World::instance().positionToIndex(ndir);

        if (neighbor == -1
        || blocktypes[neighbor] == BlockType::Air
        || blocktypes[neighbor] == BlockType::Oak_leaves
        || blocktypes[neighbor] == BlockType::Glass
        ) {
            for (int i = 0; i < vertices.size(); i+= 6)
            {
                uint32_t packed;
                uint8_t block_texture = findBlockTextures(getBlockType(blocktypes[index]), orientation);
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