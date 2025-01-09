#pragma once
#include <iostream>
#include <stdio.h>
#include <vector>
#include <deque>
#include <glm/glm.hpp>
#include <thread>
#include <mutex>
#include <asio.hpp>

namespace game
{
    struct ChunkData
    {
        glm::ivec3 pos;
        std::vector<uint8_t>blocktypes;
    };

    struct EntityData
    {
        int id;
        glm::vec3 pos;
        float yaw;
        float pitch;
        char name[64] = {};
    };

    struct Data
    {
        std::deque<ChunkData> chunks;
        std::deque<EntityData> entities;
    };

    #pragma pack(push, 1)
    struct UpdateMyEntityData
    {
        uint8_t id;
        uint32_t xpos;
        uint32_t ypos;
        uint32_t zpos;
        uint32_t yaw;
        uint32_t pitch;
    };

    struct UpdateBlockData
    {
        uint8_t id;
        uint8_t blocktype;
        uint32_t xpos;
        uint32_t ypos;
        uint32_t zpos;
    };

    struct UpdateMetaData
    {
        uint8_t id;
        uint8_t render_distance;
        uint8_t name[64];
    };
    #pragma pack(pop)

    class Client
    {
        public:
            int status;
            int asio_status;
            Data data;
            std::mutex mtx_chunk_data;
        public:
            Client();
            ~Client();
            bool receive();
            void receiveAll(size_t len);
            void clientThread();
            void startThread();
            void stopThread();

            //receive (CLIENT BOUND)
            void myEntityID();
            void addEntity();
            void receiveRemoveEntity();
            void receiveUpdateEntity();
            void receiveChunk();
            void receiveMonoTypeChunk();
            void receiveChat();
            void receiveMetaData();

            //send (SERVER BOUND)
            void sendUpdateEntity(float xpos, float ypos, float zpos, float yaw, float pitch);
            void sendUpdateBlock(uint8_t block_type, int xpos, int ypos, int zpos);
            void sendRenderDistance(uint8_t distance);
        private:
            uint8_t buffer[5000] = {0};
            int client_socket;
            int entity_id;

            asio::io_context io_context;
            asio::ip::tcp::socket asio_socket;

            UpdateMyEntityData ued = {};
            UpdateBlockData ubd = {};
            UpdateMetaData umd = {};

            char name[64] = "CzesiaLa";
            std::thread client_thread;
            std::atomic<bool> stop_flag;
            // sockaddr_in server_adress;

            void convertToFloat(float &hfloat, uint32_t data);
    };

}