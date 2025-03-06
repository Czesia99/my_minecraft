#include "ChunkMesh.hpp"
#include "World.hpp"
#include "Chunk.hpp"

namespace game
{
    ChunkMesh::ChunkMesh(const glm::ivec3 &pos) : worldpos(pos) {}

    void ChunkMesh::createChunkMesh(const std::vector<uint32_t> &vertices)
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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(uint32_t), vertices.data(), GL_STATIC_DRAW);
        vertex_count = vertices.size();
    }

    void ChunkMesh::render(const Shader &shader, const ICamera &camera)
    {
        if (vertex_count == 0)
            return;

        shader.setVec3i("chunkpos", worldpos);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    }

    void ChunkMesh::deleteChunk()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}