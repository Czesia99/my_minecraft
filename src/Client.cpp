#include "Client.hpp"
#include <cstring>
#include <arpa/inet.h>

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
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
    
        // specifying address 
        sockaddr_in server_adress;
        server_adress.sin_family = AF_INET;
        server_adress.sin_port = htons(15000);
        server_adress.sin_addr.s_addr = inet_addr("127.0.0.1"); //local server
        //server_adress.sin_addr.s_addr = inet_addr("162.19.137.231");
    
        // sending connection request
        status = connect(client_socket, (struct sockaddr*)&server_adress, sizeof(server_adress));
        
        // std::cout << "status = " << status << std::endl;
        
        // sending data
        // const char* message = "Hello, server!";
        // send(client_socket, message, strlen(message), 0);
    
        // closing socket 
        // close(client_socket); 
    }

    void Client::receive()
    {
        recv(client_socket, buffer, 1, 0);
        // receiveAll(1);
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
            default:
                break;
        }
    }

    void Client::myEntityID()
    {
        size_t bytes;
        //entityID[int]
        receiveAll(4);
        entity_id = be32toh(*(int*)&buffer);
        // std::cout << "entity id = " << entity_id << std::endl;
    }

    void Client::addEntity()
    {
        //entityID[int], xpos[float], ypos[float], yaw[float], pitch[float]
        receiveAll(24);
        // entity_id = htobe32(*(int*)&buffer); //ntohl
        // std::cout << "entity id = " << entity_id << std::endl;
    }

    void Client::removeEntity()
    {
        //entityID[int]
        receiveAll(4);
        // std::cout << "BYTES remove entity " << bytes << std::endl;
    }

    void Client::updateEntity() 
    {
        //entityID[int], xpos[float], ypos[float], yaw[float], pitch[float]
        receiveAll(24);
    }

    void Client::receiveChunk()
    {
        //chunk xpos[int] chunk ypos[int] chunk zpos[int] blocktypes[bytes[4096]]
        receiveAll(12 + 4096);
        ChunkData chunk;
        uint8_t *ptr = &buffer[0];

        memcpy(&chunk.pos.x, ptr, sizeof(int));
        chunk.pos.x = be32toh(chunk.pos.x);
        ptr += sizeof(int);

        memcpy(&chunk.pos.y, ptr, sizeof(int));
        chunk.pos.y = be32toh(chunk.pos.y);
        ptr += sizeof(int);

        memcpy(&chunk.pos.z, ptr, sizeof(int));
        chunk.pos.z = be32toh(chunk.pos.z);
        ptr += sizeof(int);

        // std::cout << "xpos: "<< chunk.xpos << " " << "ypos: "<< chunk.ypos << " " << "zpos: "<< chunk.zpos << std::endl;
        chunk.blocktypes.resize(4096);
        memcpy(&chunk.blocktypes[0], ptr, 4096);

        data.chunks.push_back(chunk);
    }

    void Client::receiveMonoTypeChunk()
    {
        //chunk xpos[int] chunk ypos[int] chunk zpos[int] blocktype[byte]
        receiveAll(12+1);
        ChunkData chunk;
        uint8_t *ptr = &buffer[0];
        uint8_t blocktype;

        memcpy(&chunk.pos.x, ptr, sizeof(int));
        chunk.pos.x = be32toh(chunk.pos.x);
        ptr += sizeof(int);

        memcpy(&chunk.pos.y, ptr, sizeof(int));
        chunk.pos.y = be32toh(chunk.pos.y);
        ptr += sizeof(int);

        memcpy(&chunk.pos.z, ptr, sizeof(int));
        chunk.pos.z = be32toh(chunk.pos.z);
        ptr += sizeof(int);
        // std::cout << "xpos: "<< chunk.xpos << " " << "ypos: "<< chunk.ypos << " " << "zpos: "<< chunk.zpos << std::endl;

        memcpy(&blocktype, ptr, sizeof(uint8_t));
        chunk.blocktypes.resize(4096);
        std::fill(std::begin(chunk.blocktypes), std::end(chunk.blocktypes), blocktype);

        data.chunks.push_back(chunk);
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
}
