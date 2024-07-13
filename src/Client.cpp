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
        receiveAll(4);
        entity_id = be32toh(*(int*)&buffer); //ntohl
        std::cout << "entity id = " << entity_id << std::endl;
    }

    void Client::addEntity()
    {
        size_t bytes;
        //entityID[int], xpos[float], ypos[float], yaw[float], pitch[float]
        receiveAll(24);
        // entity_id = htobe32(*(int*)&buffer); //ntohl
        // std::cout << "entity id = " << entity_id << std::endl;
    }

    void Client::removeEntity()
    {
        size_t bytes;
        //entityID[int]
        receiveAll(4);
        // std::cout << "BYTES remove entity " << bytes << std::endl;
    }

    void Client::updateEntity() {
        size_t bytes;
        //entityID[int], xpos[float], ypos[float], yaw[float], pitch[float]
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
        ChunkData chunk;
        // std::cout << "BYTES receive mono type chunk " << bytes << std::endl;
        uint8_t *ptr = &buffer[0];
        // int xpos = 0;
        // int ypos = 0;
        // int zpos = 0;
        uint8_t blocktype;

        memcpy(&chunk.xpos, ptr, sizeof(int));
        chunk.xpos = be32toh(chunk.xpos);
        std::cout << "xpos" << chunk.xpos << std::endl;
        ptr += sizeof(int);

        memcpy(&chunk.ypos, ptr, sizeof(int));
        chunk.ypos = be32toh(chunk.ypos);
        ptr += sizeof(int);

        memcpy(&chunk.zpos, ptr, sizeof(int));
        // be32toh(*(int *)&zpos);
        chunk.zpos = be32toh(chunk.zpos);
        ptr += sizeof(int);
        std::cout << "xpos: "<< chunk.xpos << " " << "ypos: "<< chunk.ypos << " " << "zpos: "<< chunk.zpos << std::endl;

        memcpy(&blocktype, ptr, sizeof(uint8_t));
        // htobe32(*(uint8_t *)&blocktype);
        // chunk.blockTypes = {(int)blocktype};
        std::fill(std::begin(chunk.blockTypes), std::end(chunk.blockTypes), blocktype);
        std::cout << "blocktype = " << (int)blocktype << std::endl;

        data->chunks.push_back(chunk);

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
