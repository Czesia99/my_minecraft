#pragma once
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <endian.h>

namespace game
{
    class Client
    {
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
            uint8_t buffer[4096] = {0};
            int client_socket;
            int entity_id;
            // sockaddr_in server_adress;
    };

}