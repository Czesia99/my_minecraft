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

    Chunk::Chunk(glm::ivec3 pos, const std::vector<uint8_t>&blocktypes) : chunk_worldpos(pos), blocktypes(blocktypes)
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

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(5 * sizeof(float)));
            glEnableVertexAttribArray(2);

            glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
            glEnableVertexAttribArray(3);

            // glBindVertexArray(0);
        }
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, chunk_vertices.size() * sizeof(float), chunk_vertices.data(), GL_STATIC_DRAW);
        // glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
        // glBindVertexArray(0);
    }

    void Chunk::createChunkVertices()
    {
        vertex_count = 0;
        chunk_vertices.clear();


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
        shader.use();

        glm::mat4 model = transform.getModelMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();
        glm::mat3 normal = glm::transpose(glm::inverse(model));

        shader.setMat4("model", model);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat3("normal", normal);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    }

    void Chunk::loadFaceVertices(std::vector<float> vertices, FaceOrientation orientation, glm::ivec3 &local_pos, glm::ivec3 &world_pos, int index)
    {
        glm::ivec3 direction = getFaceOrientationVector(orientation);
        glm::ivec3 ndir = direction + local_pos;
        int neighbor = positionToIndex(ndir);

        if (neighbor == -1 || blocktypes[neighbor] == 0 || blocktypes[neighbor] == 5)
        {
            for (int i = 0; i < vertices.size(); i+= 8)
            {
                chunk_vertices.push_back(vertices[i] + world_pos.x);
                chunk_vertices.push_back(vertices[i + 1] + world_pos.y);
                chunk_vertices.push_back(vertices[i + 2] + world_pos.z);

                //tex coords
                chunk_vertices.push_back(vertices[i + 3]);
                chunk_vertices.push_back(vertices[i + 4]);

                //normals
                chunk_vertices.push_back(vertices[i + 5]);
                chunk_vertices.push_back(vertices[i + 6]);
                chunk_vertices.push_back(vertices[i + 7]);

                //block textures
                chunk_vertices.push_back(findBlockTextures(getBlockType(blocktypes[index]), orientation));

                vertex_count += 1;
            }
        }
    }

    int Chunk::findBlockTextures(BlockType type, FaceOrientation orientation)
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