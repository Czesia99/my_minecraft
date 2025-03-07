#include "World.hpp"
#include "MYGL/Toolbox.hpp"
#include "Blocktype.h"

namespace game
{
    World::World()
    {
        cube_shadow = Shader("cube_shadow.vs", "cube_shadow.fs");

        loadTextureArray(block_textures_path, block_textures, 16, 16, GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST);

        cube_shadow.use();
        cube_shadow.setInt("shadowMap", 1);
    };

    World::~World()
    {
        clearAllChunks();
    }

    void World::renderTerrain(const Shader &shader, const ICamera &camera)
    {
        glm::vec4 planes[6] = {};
        extractPlanesFromProjectionViewMatrix(camera.getProjectionMatrix() * camera.getViewMatrix(), planes);

        for (const auto &[pos, chunk] : chunkMeshes)
        {
            if (boxInFrustum(planes, getChunkAABB(pos)))
            {
                chunk->render(shader, camera);
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

        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glEnable(GL_BLEND);
        renderTerrain(cube_shadow, camera);
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

        // check frustum outside/inside box
        // int out;
        // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].x > box.max.x)?1:0); if( out==8 ) return false;
        // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].x < box.min.x)?1:0); if( out==8 ) return false;
        // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].y > box.max.y)?1:0); if( out==8 ) return false;
        // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].y < box.min.y)?1:0); if( out==8 ) return false;
        // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].z > box.max.z)?1:0); if( out==8 ) return false;
        // out=0; for( int i=0; i<8; i++ ) out += ((fru.mPoints[i].z < box.min.z)?1:0); if( out==8 ) return false;

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
            chunk->deleteChunk();
            delete chunk;
        }
        chunks.clear();
    }
}