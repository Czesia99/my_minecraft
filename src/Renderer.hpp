#pragma once

#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include "glm/glm.hpp"
#include "ChunkVertices.hpp"
#include "Threadpool.hpp"

namespace game
{
    class Renderer
    {
        public:
            Renderer();
            void update();
            void addChunk(const glm::ivec3 &pos);
            void createChunksMesh();
            void createChunksVertices();
        public:
            std::unordered_set<glm::ivec3> chunks_to_update;
            std::unordered_map<glm::ivec3, ChunkVertices> chunks_vertices_to_mesh;
        private:
            ThreadPool tp = {4};
            TaskQueue tq;
            std::mutex chunks_vertices_to_mesh_mtx;
            glm::ivec3 neighbor_chunkpos[7] =
            {
                {0, 0, -1},
                {0, 0, 1},
                {-1, 0, 0},
                {1, 0, 0},
                {0, 1, 0},
                {0, -1, 0},
                {0, 0, 0}
            };


    };
}