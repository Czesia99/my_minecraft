#include "Chunk.hpp"
#include <cstring>

namespace game
{
    inline int positionToIndex(glm::ivec3 pos)
    {
        if ((pos.x < 0 || pos.x > 15) || (pos.y < 0 || pos.y > 15) || (pos.z < 0 || pos.z > 15))
            return -1;
        return pos.x + pos.y * 16 + pos.z*16*16;
    }

    Chunk::Chunk(glm::ivec3 pos, std::vector<uint8_t>&blocktypes) : blocktypes(blocktypes)
    {
        createChunkVertices(pos); // only him in thread

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

        // glVertexAttribIPointer(3, 1, GL_INT, 9 * sizeof(float), (void*)(8 * sizeof(float))); //forint
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));
        glEnableVertexAttribArray(3);

        // chunk_vertices.erase(chunk_vertices.begin(), chunk_vertices.end());
        // chunk_vertices.shrink_to_fit(); 
    }

    void Chunk::createChunkVertices(glm::ivec3 pos)
    {
        vertex_count = 0;
        for (int z = 0; z < size; z++) {
        for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {

            int index = x + y*16 + z*16*16;

            if (blocktypes[index] == 0) continue;
            bool display = false;

            glm::ivec3 local_pos = {x, y, z};
            glm::ivec3 world_pos = local_pos + pos;
            
            const std::vector<glm::ivec3> directions = {{-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1}};

            loadFrontFaceVertices(local_pos, world_pos, index);
            loadBackFaceVertices(local_pos, world_pos, index);
            loadLeftFaceVertices(local_pos, world_pos, index);
            loadRightFaceVertices(local_pos, world_pos, index);
            loadBottomFaceVertices(local_pos, world_pos, index);
            loadTopFaceVertices(local_pos, world_pos, index);
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

    void Chunk::loadFrontFaceVertices(glm::ivec3 &local_pos, glm::ivec3 &world_pos, int index)
    {
        // glm::ivec3 direction = {0, -1, 0};
        // glm::ivec3 ndir = direction + local_pos;
        // int neighbor = positionToIndex(ndir);

        // if (neighbor != -1 || blocktypes[neighbor] != 0)
        //     return;
        std::cout << "load front face" << std::endl;
        for (int i = 0; i < front_face_vertices.size(); i+= 8)
        {
            chunk_vertices.push_back(front_face_vertices[i] + world_pos.x);
            chunk_vertices.push_back(front_face_vertices[i + 1] + world_pos.y);
            chunk_vertices.push_back(front_face_vertices[i + 2] + world_pos.z);

            //tex coords
            chunk_vertices.push_back(front_face_vertices[i + 3]);
            chunk_vertices.push_back(front_face_vertices[i + 4]);

            //normals
            chunk_vertices.push_back(front_face_vertices[i + 5]);
            chunk_vertices.push_back(front_face_vertices[i + 6]);
            chunk_vertices.push_back(front_face_vertices[i + 7]);

            //block type
            chunk_vertices.push_back(blocktypes[index] - 1);
            
            vertex_count += 1;
        }
    }

    void Chunk::loadBackFaceVertices(glm::ivec3 &local_pos, glm::ivec3 &world_pos, int index)
    {
        // glm::ivec3 direction = {0, 0, 1};
        // glm::ivec3 ndir = direction + local_pos;
        // int neighbor = positionToIndex(ndir);

        // if (neighbor != -1 || blocktypes[neighbor] != 0)
        //     return;
        std::cout << "load back face" << std::endl;
        for (int i = 0; i < back_face_vertices.size(); i+= 8)
        {
            chunk_vertices.push_back(back_face_vertices[i] + world_pos.x);
            chunk_vertices.push_back(back_face_vertices[i + 1] + world_pos.y);
            chunk_vertices.push_back(back_face_vertices[i + 2] + world_pos.z);

            //tex coords
            chunk_vertices.push_back(back_face_vertices[i + 3]);
            chunk_vertices.push_back(back_face_vertices[i + 4]);

            //normals
            chunk_vertices.push_back(back_face_vertices[i + 5]);
            chunk_vertices.push_back(back_face_vertices[i + 6]);
            chunk_vertices.push_back(back_face_vertices[i + 7]);

            //block type
            chunk_vertices.push_back(blocktypes[index] - 1);
            
            vertex_count += 1;
        }
    }

    void Chunk::loadLeftFaceVertices(glm::ivec3 &local_pos, glm::ivec3 &world_pos, int index)
    {
        glm::ivec3 direction = {-1, 0, 0};
        glm::ivec3 ndir = direction + local_pos;
        int neighbor = positionToIndex(ndir);

        if (neighbor != -1 || blocktypes[neighbor] != 0)
            return;
        std::cout << "load left face" << std::endl;
        for (int i = 0; i < left_face_vertices.size(); i+= 8)
        {
            chunk_vertices.push_back(left_face_vertices[i] + world_pos.x);
            chunk_vertices.push_back(left_face_vertices[i + 1] + world_pos.y);
            chunk_vertices.push_back(left_face_vertices[i + 2] + world_pos.z);

            //tex coords
            chunk_vertices.push_back(left_face_vertices[i + 3]);
            chunk_vertices.push_back(left_face_vertices[i + 4]);

            //normals
            chunk_vertices.push_back(left_face_vertices[i + 5]);
            chunk_vertices.push_back(left_face_vertices[i + 6]);
            chunk_vertices.push_back(left_face_vertices[i + 7]);

            //block type
            chunk_vertices.push_back(blocktypes[index] - 1);
            
            vertex_count += 1;
        }
    }

    void Chunk::loadRightFaceVertices(glm::ivec3 &local_pos, glm::ivec3 &world_pos, int index)
    {
        glm::ivec3 direction = {1, 0, 0};
        glm::ivec3 ndir = direction + local_pos;
        int neighbor = positionToIndex(ndir);

        if (neighbor != -1 || blocktypes[neighbor] != 0)
            return;
        std::cout << "load right face" << std::endl;
        for (int i = 0; i < right_face_vertices.size(); i+= 8)
        {
            chunk_vertices.push_back(right_face_vertices[i] + world_pos.x);
            chunk_vertices.push_back(right_face_vertices[i + 1] + world_pos.y);
            chunk_vertices.push_back(right_face_vertices[i + 2] + world_pos.z);

            //tex coords
            chunk_vertices.push_back(right_face_vertices[i + 3]);
            chunk_vertices.push_back(right_face_vertices[i + 4]);

            //normals
            chunk_vertices.push_back(right_face_vertices[i + 5]);
            chunk_vertices.push_back(right_face_vertices[i + 6]);
            chunk_vertices.push_back(right_face_vertices[i + 7]);

            //block type
            chunk_vertices.push_back(blocktypes[index] - 1);
            
            vertex_count += 1;
        }
    }

    void Chunk::loadTopFaceVertices(glm::ivec3 &local_pos, glm::ivec3 &world_pos, int index)
    {
        glm::ivec3 direction = {0, -1, 0};
        glm::ivec3 ndir = direction + local_pos;
        int neighbor = positionToIndex(ndir);
        // std::cout << " ndir = " << ndir.x << " " << ndir.y << " " << ndir.z << std::endl;
        if (neighbor != -1 || blocktypes[neighbor] != 0)
            return;

        std::cout << "load top face" << std::endl;
        std::cout << "blocktype = " << (int)blocktypes[neighbor] << std::endl;
        for (int i = 0; i < top_face_vertices.size(); i+= 8)
        {
            chunk_vertices.push_back(top_face_vertices[i] + world_pos.x);
            chunk_vertices.push_back(top_face_vertices[i + 1] + world_pos.y);
            chunk_vertices.push_back(top_face_vertices[i + 2] + world_pos.z);

            //tex coords
            chunk_vertices.push_back(top_face_vertices[i + 3]);
            chunk_vertices.push_back(top_face_vertices[i + 4]);

            //normals
            chunk_vertices.push_back(top_face_vertices[i + 5]);
            chunk_vertices.push_back(top_face_vertices[i + 6]);
            chunk_vertices.push_back(top_face_vertices[i + 7]);

            //block type
            chunk_vertices.push_back(blocktypes[index] - 1);
            
            vertex_count += 1;
        }
    }

    void Chunk::loadBottomFaceVertices(glm::ivec3 &local_pos, glm::ivec3 &world_pos, int index)
    {
        // glm::ivec3 direction = {0, 1, 0};
        // glm::ivec3 ndir = direction + local_pos;
        // int neighbor = positionToIndex(ndir);

        // if (neighbor != -1 || blocktypes[neighbor] != 0)
        //     return;
        // std::cout << "load bottom face" << std::endl;
        for (int i = 0; i < bottom_face_vertices.size(); i+= 8)
        {
            chunk_vertices.push_back(bottom_face_vertices[i] + world_pos.x);
            chunk_vertices.push_back(bottom_face_vertices[i + 1] + world_pos.y);
            chunk_vertices.push_back(bottom_face_vertices[i + 2] + world_pos.z);

            //tex coords
            chunk_vertices.push_back(bottom_face_vertices[i + 3]);
            chunk_vertices.push_back(bottom_face_vertices[i + 4]);

            //normals
            chunk_vertices.push_back(bottom_face_vertices[i + 5]);
            chunk_vertices.push_back(bottom_face_vertices[i + 6]);
            chunk_vertices.push_back(bottom_face_vertices[i + 7]);

            //block type
            chunk_vertices.push_back(blocktypes[index] - 1);
            
            vertex_count += 1;
        }
    }

    void Chunk::deleteChunk()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}

// vertices.insert(vertices.end(), {
//                 x+0.f, y+0.f, z+0.f, 0.f, 0.f, Orientation::Front,
//                 x+1.f, y+0.f, z+0.f, 1.f, 0.f, Orientation::Front,
//                 x+1.f, y+1.f, z+0.f, 1.f, 1.f, Orientation::Front,
//                 x+0.f, y+1.f, z+0.f, 0.f, 1.f, Orientation::Front,
//             });