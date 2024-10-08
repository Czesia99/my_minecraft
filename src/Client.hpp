#pragma once
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <deque>
#include <glm/glm.hpp>
#include <mutex>

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
        glm::ivec3 pos;
        float yaw;
        float pitch;
    };

    struct Data 
    {
        std::deque<ChunkData> chunks;
        std::deque<EntityData> entitys;
    };

    struct UpdateEntityData
    {
        uint8_t id;
        uint32_t xpos;
        uint32_t ypos;
        uint32_t zpos;
        uint32_t yaw;
        uint32_t pitch;
    } __attribute__((packed));

    struct UpdateBlockData
    {
        uint8_t id;
        uint8_t blocktype;
        uint32_t xpos;
        uint32_t ypos;
        uint32_t zpos;
    } __attribute__((packed));

    class Client
    {
        public:
            int status;
            Data data;
            std::mutex mtx_chunk_data;
        public:
            Client();
            void receive();
            void receiveAll(size_t len);
            void receiveThread();

            //receive (CLIENT BOUND)
            void myEntityID();
            void addEntity();
            void receiveRemoveEntity();
            void receiveUpdateEntity();
            void receiveChunk();
            void receiveMonoTypeChunk();

            //send (SERVER BOUND)
            void sendUpdateEntity(float xpos, float ypos, float zpos, float yaw, float pitch);
            void sendUpdateBlock(uint8_t block_type, int xpos, int ypos, int zpos);
            
        private:
            uint8_t buffer[5000] = {0};
            int client_socket;
            int entity_id;
            UpdateEntityData ued = {};
            UpdateBlockData ubd = {};
            char name[64] = "Czesia";
            // sockaddr_in server_adress;
    };

}