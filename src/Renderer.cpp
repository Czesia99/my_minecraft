#include "Renderer.hpp"
#include "World.hpp"

namespace game
{
    Renderer::Renderer() {}
    Renderer::~Renderer()
    {
        tp.stop();
    }

    void Renderer::update()
    {
        createChunksVertices();
        createChunksMesh();
    }

    void Renderer::addChunk(const glm::ivec3 &pos)
    {
        for (auto &offsetpos : neighbor_chunkpos)
        {
            glm::ivec3 cpos = pos + offsetpos * 16;
            chunks_to_update.insert(cpos);
        }
    }

    void Renderer::createChunksVertices()
    {
        for (auto &ctu : chunks_to_update)
        {
            tp.enqueue([ctu, this] {
                    ChunkVertices chunk_vertices;
                    chunk_vertices.createChunkVertices(ctu);

                    chunks_vertices_to_mesh_mtx.lock();
                    chunks_vertices_to_mesh[ctu] = chunk_vertices;
                    chunks_vertices_to_mesh_mtx.unlock();
            });
        }
        chunks_to_update.clear();
    }

    void Renderer::createChunksMesh()
    {
        chunks_vertices_to_mesh_mtx.lock();
        for (auto &[pos, vertices] : chunks_vertices_to_mesh)
        {
            auto it = World::instance().chunkMeshes.find(pos);
            if (it != World::instance().chunkMeshes.end())
            {
                it->second.createChunkMesh(vertices.chunk_vertices);
            }
        }
        chunks_vertices_to_mesh.clear();
        chunks_vertices_to_mesh_mtx.unlock();
    }
}