#pragma once
#include <iostream>
#include <stdio.h>
#include <vector>
#include <deque>
#include <glm/glm.hpp>
#include <thread>
#include <mutex>
#include <asio.hpp>
#include <atomic>
#include <map>

namespace game
{
    struct ChunkData
    {
        glm::ivec3 pos;
        std::array<uint8_t, 4096> blocktypes;
    };

    struct EntityData
    {
        int id;
        glm::vec3 pos;
        float yaw;
        float pitch;
        char name[64] = {};
    };

    struct ChatData
    {
        std::string text;
    };

    struct Data
    {
        std::deque<ChunkData> chunks;
        std::deque<EntityData> entities;
        std::deque<ChatData> chat_history;
        std::deque<int> rm_entity;
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

    struct SendChatData
    {
        uint8_t id;
        char message[4096];
    };

    #pragma pack(pop)

    class Client
    {
        public:
            int status;
            int asio_status;
            Data data;
            std::mutex data_mtx;
        public:
            Client();
            ~Client();
            bool receive();
            bool receiveAll(size_t len);
            void clientThread();
            void startThread();
            void stopThread();

            //receive (CLIENT BOUND)
            void myEntityID();
            void addEntity();
            void removeEntity();
            void receiveUpdateEntity();
            void receiveChunk();
            void receiveMonoTypeChunk();
            void receiveChat();
            void receiveEntityMetaData();

            //send (SERVER BOUND)
            void sendUpdateEntity(float xpos, float ypos, float zpos, float yaw, float pitch);
            void sendUpdateBlock(uint8_t block_type, int xpos, int ypos, int zpos);
            void sendRenderDistance(uint8_t distance);
            void sendTextChat(const char *input);
        private:
            uint8_t buffer[5000] = {0};
            int client_socket;
            std::atomic<bool> receive_in_progress;
            int entity_id;

            asio::io_context io_context;
            asio::ip::tcp::socket asio_socket;

            UpdateMyEntityData ued = {};
            UpdateBlockData ubd = {};
            UpdateMetaData umd = {};

            char name[64] = "CzesiaLa";
            std::thread client_thread;
            std::atomic<bool> stop_flag;
            void cancelCurrentOperations();

            void convertToFloat(float &hfloat, uint32_t data);

            std::unordered_map<uint8_t, size_t> packetid = {
                {0x00, 4},
                {0x01, sizeof(EntityData)},
                {0x02, 4},
                {0x03, 24},
                {0x04, 12 + 4096},
                {0x05, 13},
                {0x06, 4096},
                {0x07, 68}
            };
    };

}
