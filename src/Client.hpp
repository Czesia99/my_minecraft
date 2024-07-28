#pragma once
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <deque>
#include <glm/glm.hpp>

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

    class Client
    {
        public:
            Data data;
        public:
            Client();
            void receive();
            void receiveAll(size_t len);
            void receiveThread();

            //receive
            void myEntityID();
            void addEntity();
            void receiveRemoveEntity();
            void receiveUpdateEntity();
            void receiveChunk();
            void receiveMonoTypeChunk();

            //send
            void sendUpdateEntity(float &xpos, float &ypos, float &zpos, float &yaw, float &pitch);
            void sendUpdateBlock();
            
        private:
            uint8_t buffer[5000] = {0};
            int client_socket;
            int entity_id;
            // sockaddr_in server_adress;
    };

}