#include "Chunk.hpp"

namespace game
{
    Chunk::Chunk(glm::ivec3 pos)
    {
        std::cout << chunk_vertices.size() << std::endl;
        std::cout << cube_vertices.size() << std::endl;

        for (int i = 0; i < 4096; i++)
        {
            int val = rand() % 3;
            blocks[i] = val;

            // std::cout << "zval = " << val << std::endl;
        }

        createChunkVertices();
        std::cout << "j'aime le caca, c'est délicieux... le caca hmmmmmmmmmmmmmmmmmmmmmmmmmmm" << std::endl;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, chunk_vertices.size() * sizeof(float), chunk_vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glVertexAttribIPointer(3, 1, GL_INT, 9 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);
    }

    void Chunk::createChunkVertices()
    {
        vertex_count = 0;
        std::cout << "start chunk vertices setup" << std::endl;
        for (int z = 0; z < size; z++) {
        for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {

            int index = x + y*16 + z*16*16;

            if (blocks[index] == 0) continue;

            glm::vec3 cube_pos = {x, y, z};

            for (int i = 0; i < cube_vertices.size(); i += 8)
            {
                //positions
                chunk_vertices.push_back(cube_vertices[i] + cube_pos.x);
                chunk_vertices.push_back(cube_vertices[i + 1] + cube_pos.y);
                chunk_vertices.push_back(cube_vertices[i + 2] + cube_pos.z);

                //tex coords
                chunk_vertices.push_back(cube_vertices[i + 3]);
                chunk_vertices.push_back(cube_vertices[i + 4]);

                //normals
                chunk_vertices.push_back(cube_vertices[i + 5]);
                chunk_vertices.push_back(cube_vertices[i + 6]);
                chunk_vertices.push_back(cube_vertices[i + 7]);

                //block type ?
                chunk_vertices.push_back(blocks[index] - 1);
                
                vertex_count += 1;
            }
        }}}

        std::cout << "vertex count" << vertex_count << std::endl;
        std::cout << "end chunk vertices setup" << std::endl;
    }

    void Chunk::render(const Shader &shader, const ICamera &camera)
    {
        shader.use();

        // if (diffuse_texture)
        // {
        //     shader.setInt("material.diffuse", 0);
        //     glActiveTexture(GL_TEXTURE0);
        //     glBindTexture(GL_TEXTURE_2D, diffuse_texture);
        // }

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
}