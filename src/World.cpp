#include "World.hpp"

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
        for (const auto &it : chunks)
        {
            it.second->render(shader, camera);
        }
    }

    void World::render(const Camera3D &camera, const int &width, const int &height)
    {
        shadowmap.renderShadowmap(camera);

        glEnable(GL_CULL_FACE);
        cube_shadow.use();
        cube_shadow.setMat4("lightSpaceMatrix", shadowmap.lightSpaceMatrix);
        cube_shadow.setVec3("lightDir", shadowmap.lightDir);
        cube_shadow.setVec3("viewPos", camera.position);
        cube_shadow.setMat4("projection", camera.getProjectionMatrix());
        cube_shadow.setMat4("view", camera.getViewMatrix());
        glBindTextureUnit(1, shadowmap.depthMap);
        glBindTextureUnit(0, block_textures);

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
            uint8_t blocktype = it->second->blocktypes[it->second->positionToIndex(local_pos)];
            return blocktype;
        } else
            return 0;
    }

    uint8_t World::getBlockAt(float x, float y, float z)
    {
        return getBlockAt(int(glm::floor(x)), int(glm::floor(y)), int(glm::floor(z)));
    }

    void World::clearAllChunks()
    {
        for (auto &[pos, chunk] : chunks)
        {
            chunk->deleteChunk();
            delete chunk;
        }
        chunks.clear();
    }
}