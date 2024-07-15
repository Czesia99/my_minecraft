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
        int xpos;
        int ypos;
        int zpos;
        glm::ivec3 pos;
        std::vector<uint8_t>blocktypes;
    };

    struct Data 
    {
        std::deque<ChunkData> chunks;
    };

    class Client
    {
        public:
            Data data;
        public:
            Client();
            void receive();
            void connexion();

            void myEntityID();
            void addEntity();
            void removeEntity();
            void updateEntity();
            void receiveChunk();
            void receiveMonoTypeChunk();

            void receiveAll(size_t len);
        private:
            uint8_t buffer[5000] = {0};
            int client_socket;
            int entity_id;
            // sockaddr_in server_adress;
    };

}