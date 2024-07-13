#include "Client.hpp"
#include <cstring> 

void print_buffer(const char *title, const unsigned char *buf, size_t buf_len)
{
    printf("%s [ ", title);
    for (size_t i = 0 ; i < buf_len ; ++i) {
        printf("%02X%s", buf[i], ( i + 1 ) % 16 == 0 ? "\r\n" : " " );
    }
    printf("]\n");
}

inline void swapEndian(int &val)
{
    val = (val<<24) | ((val<<8) & 0x00ff0000) | ((val>>8) & 0x0000ff00) | (val>>24);
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
                break;
            case 0x01:
                addEntity();
                break;
            case 0x02:
                removeEntity();
                break;
            case 0x03:
                updateEntity();
                break;
            case 0x04:
                receiveChunk();
                break;
            case 0x05:
                receiveMonoTypeChunk();
                break;
        }
    }

    void Client::myEntityID()
    {
        size_t bytes;
        //entityID[int]
        // bytes = recv(client_socket, buffer, 4, 0);
        // std::cout << "BYTES entity id " << bytes << std::endl;
        receiveAll(4);
        entity_id = be32toh(*(int*)&buffer); //ntohl
        std::cout << "entity id = " << entity_id << std::endl;
    }

    void Client::addEntity()
    {
        size_t bytes;
        //entityID[int], xpos[float], ypos[float], yaw[float], pitch[float]
        // bytes = recv(client_socket, buffer, 24, 0);
        // std::cout << "BYTES add entity " << bytes << std::endl;
        receiveAll(24);
        // entity_id = htobe32(*(int*)&buffer); //ntohl
        // std::cout << "entity id = " << entity_id << std::endl;
    }

    void Client::removeEntity()
    {
        size_t bytes;
        //entityID[int]
        // bytes = recv(client_socket, buffer, 4, 0);
        receiveAll(4);
        // std::cout << "BYTES remove entity " << bytes << std::endl;
    }

    void Client::updateEntity() {
        size_t bytes;
        //entityID[int], xpos[float], ypos[float], yaw[float], pitch[float]
        // bytes = recv(client_socket, buffer, 24, 0);
        // std::cout << "BYTES update entity " << bytes << std::endl;
        receiveAll(24);
    }

    void Client::receiveChunk()
    {
        size_t bytes;
        // //chunk xpos[int] chunk ypos[int] chunk zpos[int] blocktypes[bytes[4096]]
        // bytes = recv(client_socket, buffer, 12 + 4096, 0);
        receiveAll(12 + 4096);
        // std::cout << "BYTES receive chunk " << bytes << std::endl;
        // uint8_t *ptr = buffer;
        // uint8_t xpos;
        // memcpy(&xpos, ptr, sizeof(xpos));
        // be32toh(*(int *)&xpos);
        // // SwapEndian(xpos);
        // ptr += sizeof(xpos);
        // std::cout << "xpos" << xpos << std::endl;
    }

    void Client::receiveMonoTypeChunk()
    {
        // size_t bytes;
        //chunk xpos[int] chunk ypos[int] chunk zpos[int] blocktype[byte]
        // bytes = recv(client_socket, buffer, 12 + 1, 0);
        receiveAll(12+1);

        // std::cout << "BYTES receive mono type chunk " << bytes << std::endl;
        uint8_t *ptr = &buffer[0];
        int xpos = 0;
        int ypos = 0;
        int zpos = 0;
        uint8_t blocktype;

        memcpy(&xpos, ptr, sizeof(int));
        xpos = be32toh(xpos);
        std::cout << "xpos" << xpos << std::endl;
        ptr += sizeof(int);

        memcpy(&ypos, ptr, sizeof(int));
        ypos = be32toh(ypos);
        ptr += sizeof(int);

        memcpy(&zpos, ptr, sizeof(int));
        // be32toh(*(int *)&zpos);
        zpos = be32toh(zpos);
        ptr += sizeof(int);
        std::cout << "xpos: "<< xpos << " " << "ypos: "<< ypos << " " << "zpos: "<< zpos << std::endl;

        memcpy(&blocktype, ptr, sizeof(uint8_t));
        // htobe32(*(uint8_t *)&blocktype);
        std::cout << "blocktype = " << blocktype << std::endl;
        
    }
    void Client::connexion()
    {
        
    }

    void Client::receiveAll(size_t len)
    {
        size_t bytes = 0;
        
        while (bytes != len){
            bytes += recv(client_socket, &buffer[bytes], len - bytes, 0);
        }
        std::cout << "bytes = " << bytes << std::endl;
    }

    // void Client::receiveAll(int fd, uint8_t *buffer, size_t size)
    // {
    //     size_t bytes_received = 0;

    //     while (1) {
    //         int recv_size = recv(fd, &buffer[bytes_received], size - bytes_received, 0);
    //         bytes_received += recv_size;

    //         if (recv_size == -1)
    //             return -1;

    //         if (bytes_received == size)
    //             break;
    //     }

    //     return bytes_received;
    // }
}
