#include "Chunk.hpp"
#include <cstring>
#include "Blocktype.h"

namespace game
{
    int Chunk::positionToIndex(glm::ivec3 pos)
    {
        if (pos.x < 0 || pos.x > 15 || pos.y < 0 || pos.y > 15 || pos.z < 0 || pos.z > 15)
            return -1;
        return pos.x + pos.y * 16 + pos.z*16*16;
    }

    Chunk::Chunk(const glm::ivec3 &pos, const std::vector<uint8_t>&blocktypes) : chunk_worldpos(pos), blocktypes(blocktypes)
    {
        // createChunkVertices(pos);
        // createChunkMesh();
    }

    void Chunk::createChunkMesh()
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
        // glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
        // glBindVertexArray(0);
        // packed_vertices.clear();
        packed_vertices.shrink_to_fit();
    }

    void Chunk::createChunkVertices()
    {
        vertex_count = 0;
        // chunk_vertices.clear();
        packed_vertices.clear();


        for (int z = 0; z < size; z++) {
        for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {

            int index = x + y*16 + z*16*16;

            if (blocktypes[index] == 0) continue;
            bool display = false;

            glm::ivec3 local_pos = {x, y, z};
            glm::ivec3 world_pos = local_pos + chunk_worldpos;

            loadFaceVertices(front_face_vertices, FaceOrientation::Front, local_pos, world_pos, index);
            loadFaceVertices(back_face_vertices, FaceOrientation::Back, local_pos, world_pos, index);
            loadFaceVertices(left_face_vertices, FaceOrientation::Left, local_pos, world_pos, index);
            loadFaceVertices(right_face_vertices, FaceOrientation::Right, local_pos, world_pos, index);
            loadFaceVertices(top_face_vertices, FaceOrientation::Top, local_pos, world_pos, index);
            loadFaceVertices(bottom_face_vertices, FaceOrientation::Bottom, local_pos, world_pos, index);
        }}}
    }

    void Chunk::render(const Shader &shader, const ICamera &camera)
    {
        shader.setVec3i("chunkpos", chunk_worldpos);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    }

    // uint32_t Chunk::packVerticesData(uint8_t x, uint8_t y, uint8_t z, uint8_t tx, )
    // {
    //     uint32_t packed;


    // }

    void Chunk::loadFaceVertices(const std::vector<uint8_t> &vertices, FaceOrientation orientation, const glm::ivec3 &local_pos, const glm::ivec3 &world_pos, int index)
    {
        const glm::ivec3 direction = getFaceOrientationVector(orientation);
        const glm::ivec3 ndir = direction + local_pos;
        int neighbor = positionToIndex(ndir);

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

                // std::cout << "Packed value: " << packed << std::endl;
                packed_vertices.push_back(packed);
                vertex_count += 1;
            }
        }
    }

    uint8_t Chunk::findBlockTextures(BlockType type, FaceOrientation orientation)
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

    void Chunk::deleteChunk()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}