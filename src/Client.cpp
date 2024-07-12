#include "Client.hpp"

void print_buffer(const char *title, const unsigned char *buf, size_t buf_len)
{
    printf("%s [ ", title);
    for (size_t i = 0 ; i < buf_len ; ++i) {
        printf("%02X%s", buf[i], ( i + 1 ) % 16 == 0 ? "\r\n" : " " );
    }
    printf("]\n");
}

namespace game
{
    Client::Client()
    {
        int status;
        // creating socket
        // int client_socket;
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
        // specifying address 
        sockaddr_in server_adress;
        server_adress.sin_family = AF_INET; 
        server_adress.sin_port = htons(15000);
        server_adress.sin_addr.s_addr = INADDR_ANY; 
    
        // sending connection request
        status = connect(client_socket, (struct sockaddr*)&server_adress, sizeof(server_adress));
        
        std::cout << "status = " << status << std::endl;
        
        // sending data
        // const char* message = "Hello, server!";
        // send(client_socket, message, strlen(message), 0);

        // recieving data 

    
        // closing socket 
        // close(client_socket); 
    }

    void Client::receive()
    {
        recv(client_socket, buffer, 1, 0);
        print_buffer("buffer", buffer, 1);
        uint8_t id = buffer[0];
        switch (id) 
        {
            case 0x00:
                myEntityID();
            // case 0x01:
            //     addEntity();
            // case 0x02:
            //     removeEntity();
            // case 0x03:
            //     updateEntity();
            // case 0x04:
            //     receiveChunk();
            // case 0x05:
            //     receiveMonoTypeChunk();
        }
    }

    void Client::myEntityID()
    {
        recv(client_socket, buffer, 4, 0);
        entity_id = htobe32(*(int*)&buffer);
        std::cout << "entity id = " << entity_id << std::endl;
    }

    void Client::connexion()
    {

    }
}
