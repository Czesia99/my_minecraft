#pragma once
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h> 

namespace game
{
    class Client
    {
        public:
            Client();
            void receive();
            void connexion();
        private:
            int client_socket;
            // const int 
            // sockaddr_in server_adress;
    };

}