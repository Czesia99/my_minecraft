#include "World.hpp"
#include "MYGL/Toolbox.hpp"
#include "Blocktype.h"

namespace game
{
    World::World()
    {
        glGenVertexArrays(1, &chunk_vao);
        glBindVertexArray(chunk_vao);

        glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, sizeof(uint32_t), (void*)0);
        glEnableVertexAttribArray(0);

        cube_shadow = Shader("cube_shadow.vs", "cube_shadow.fs");

        loadTextureArray(block_textures_path, block_textures, 16, 16, GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST);

        cube_shadow.use();
        cube_shadow.setInt("shadowMap", 1);

    };

    World::~World()
    {
        clearAllChunks();
        glDeleteVertexArrays(1, &chunk_vao);
    }

    void World::renderTerrain(const Shader &shader, const ICamera &camera)
    {
        glm::vec4 planes[6] = {};
        extractPlanesFromProjectionViewMatrix(camera.getProjectionMatrix() * camera.getViewMatrix(), planes);

        glBindVertexArray(chunk_vao);
        for (const auto &[pos, chunk] : chunkMeshes)
        {
            if (chunk.vbo != 0)
            {
                if (boxInFrustum(planes, getChunkAABB(pos)))
                {
                    glBindVertexBuffer(0, chunk.vbo, 0, 4);
                    chunk.render(shader, camera);
                }
            }
        }
    }

    void World::render(const Camera3D &camera, const int &width, const int &height)
    {
        shadowmap.renderShadowmap(camera);

        glEnable(GL_CULL_FACE);
        cube_shadow.use();
        cube_shadow.setBool("fogDisplay", fog_display);
        cube_shadow.setVec4("fogColor", fog_color);
        cube_shadow.setFloat("fogMaxDist", fog_maxdist);
        cube_shadow.setFloat("fogMinDist", fog_mindist);
        cube_shadow.setMat4("lightSpaceMatrix", shadowmap.lightSpaceMatrix);
        cube_shadow.setVec3("lightDir", shadowmap.light_dir);
        cube_shadow.setVec3("viewPos", camera.position);
        cube_shadow.setMat4("projection", camera.getProjectionMatrix());
        cube_shadow.setMat4("view", camera.getViewMatrix());
        glBindTextureUnit(1, shadowmap.depthMap);
        glBindTextureUnit(0, block_textures);

        renderTerrain(cube_shadow, camera);
    }

    void World::createOrReplaceChunk(const glm::ivec3 &pos, const std::array<uint8_t, 4096> &blocktypes)
    {
        Chunk chunk;
        chunk.worldpos = pos;
        chunk.blocktypes = blocktypes;

        chunk_mtx.lock();
        auto it = chunks.find(chunk.worldpos);
        if (it != chunks.end())
        {
            it->second = chunk;
        } else {
            chunks[chunk.worldpos] = chunk;
            ChunkMesh chunkmesh{chunk.worldpos};
            chunkMeshes[chunk.worldpos] = chunkmesh;
        }
        chunk_mtx.unlock();
    }

    uint8_t World::getBlockAt(int x, int y, int z)
    {
        glm::ivec3 chunk_pos = glm::floor(glm::vec3(x,y,z) / 16.0f) * 16.0f;
        glm::ivec3 local_pos = {x % 16, y % 16, z % 16};
        if (local_pos.x < 0) local_pos.x += 16;
        if (local_pos.y < 0) local_pos.y += 16;
        if (local_pos.z < 0) local_pos.z += 16;

        glm::ivec3 cube_pos = {x, y, z};

        auto it =  chunks.find(chunk_pos);
        if (it != chunks.end())
        {
            uint8_t blocktype = it->second.blocktypes[positionToIndex(local_pos)];
            return blocktype;
        } else
            return 0;
    }

    uint8_t World::getBlockAt(float x, float y, float z)
    {
        return getBlockAt(int(glm::floor(x)), int(glm::floor(y)), int(glm::floor(z)));
    }

    World::ChunkAABB World::getChunkAABB(const glm::ivec3 &pos)
    {
        ChunkAABB aabb = {};
        aabb.min = pos;
        aabb.max = pos + 16;
        return aabb;
    }

    bool World::boxInFrustum( const glm::vec4 planes[6], ChunkAABB const &box)
    {
        // check box outside/inside of frustum
        for( int i=0; i<6; i++ )
        {
            int out = 0;
            out += ((glm::dot( planes[i], glm::vec4(box.min.x, box.min.y, box.min.z, 1.0f) ) < 0.0 )?1:0);
            out += ((glm::dot( planes[i], glm::vec4(box.max.x, box.min.y, box.min.z, 1.0f) ) < 0.0 )?1:0);
            out += ((glm::dot( planes[i], glm::vec4(box.min.x, box.max.y, box.min.z, 1.0f) ) < 0.0 )?1:0);
            out += ((glm::dot( planes[i], glm::vec4(box.max.x, box.max.y, box.min.z, 1.0f) ) < 0.0 )?1:0);
            out += ((glm::dot( planes[i], glm::vec4(box.min.x, box.min.y, box.max.z, 1.0f) ) < 0.0 )?1:0);
            out += ((glm::dot( planes[i], glm::vec4(box.max.x, box.min.y, box.max.z, 1.0f) ) < 0.0 )?1:0);
            out += ((glm::dot( planes[i], glm::vec4(box.min.x, box.max.y, box.max.z, 1.0f) ) < 0.0 )?1:0);
            out += ((glm::dot( planes[i], glm::vec4(box.max.x, box.max.y, box.max.z, 1.0f) ) < 0.0 )?1:0);
            if( out==8 ) return false;
        }

        return true;
    }

    int World::positionToIndex(glm::ivec3 pos)
    {
        if (pos.x < 0 || pos.x > 15 || pos.y < 0 || pos.y > 15 || pos.z < 0 || pos.z > 15)
            return -1;
        return pos.x + pos.y * 16 + pos.z*16*16;
    }

    void World::clearAllChunks()
    {
        for (auto &[pos, chunk] : chunkMeshes)
        {
            chunk.deleteChunk();
        }
        chunkMeshes.clear();
    }
}