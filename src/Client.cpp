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
    Client::Client() : stop_flag(false)
    {
        client_socket = socket(AF_INET, SOCK_STREAM, 0);

        // specifying address
        sockaddr_in server_adress;
        server_adress.sin_family = AF_INET;
        server_adress.sin_port = htons(15000);
        // server_adress.sin_addr.s_addr = inet_addr("127.0.0.1");
        server_adress.sin_addr.s_addr = inet_addr("162.19.137.231");

        // sending connection request
        status = connect(client_socket, (struct sockaddr*)&server_adress, sizeof(server_adress));

        if (status == -1)
        {
            std::cout << "can't connect to game server" << std::endl;
        }
        // close(client_socket);
    }

    Client::~Client()
    {
        stopThread();
        if (client_thread.joinable()) {
            client_thread.join();
        }
    }

    void Client::clientThread()
    {
        while(!stop_flag)
        {
            if (!receive()) {
                continue;
            }
        }
    }

    void Client::startThread()
    {
        client_thread = std::thread(&Client::clientThread, this);
    }

    void Client::stopThread()
    {
        stop_flag = true;
    }

    bool Client::receive()
    {
        // asio::ip::tcp::socket socket = asio_socket;
        // uint8_t buffer[1];

        fd_set readfds;
        struct timeval timeout;

        FD_ZERO(&readfds);
        FD_SET(client_socket, &readfds);

        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int result = select(client_socket + 1, &readfds, NULL, NULL, &timeout);

        if (result > 0) {
            recv(client_socket, buffer, 1, 0);

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
                    receiveRemoveEntity();
                    break;
                case 0x03:
                    receiveUpdateEntity();
                    break;
                case 0x04:
                    receiveChunk();
                    break;
                case 0x05:
                    receiveMonoTypeChunk();
                    break;
                case 0x06:
                    receiveChat();
                    break;
                case 0x07:
                    receiveMetaData();
                    break;
                default:
                    break;
            }

            return true;

        } else if (result == 0) {
            return false;
        } else {
            return false;
        }
    }

    void Client::receiveAll(size_t len)
    {
        size_t bytes = 0;

        while (bytes != len){
            bytes += recv(client_socket, &buffer[bytes], len - bytes, 0);
        }
    }

    void Client::myEntityID()
    {
        std::cout << "receive my entity id" << std::endl;
        //entityID[int]
        receiveAll(4);
        entity_id = be32toh(*(int*)&buffer);
        std::cout << "my entity id == " << entity_id << std::endl;
    }

    void Client::convertToFloat(float &hfloat, uint32_t data)
    {
        data = be32toh(data);
        std::memcpy(&hfloat, &data, sizeof(float));
    }

    void Client::addEntity()
    {
        //entityID[int], xpos[float], ypos[float], zpos[float], yaw[float], pitch[float]
        receiveAll(24 + 64);
        EntityData entity {};
        uint8_t *ptr = &buffer[0];

        memcpy(&entity.id, ptr, sizeof(int));
        // std::cout << "ID without conversion: " << entity.id << std::endl;
        entity.id = be32toh(entity.id);
        std::cout << "ID with conversion: " << entity.id << std::endl;
        ptr += sizeof(int);

        uint32_t hold_posx;
        memcpy(&hold_posx, ptr, sizeof(uint32_t));
        convertToFloat(entity.pos.x, hold_posx);
        ptr += sizeof(uint32_t);

        uint32_t hold_posy;
        memcpy(&hold_posy, ptr, sizeof(uint32_t));
        convertToFloat(entity.pos.y, hold_posy);
        ptr += sizeof(uint32_t);

        uint32_t hold_posz;
        memcpy(&hold_posz, ptr, sizeof(uint32_t));
        convertToFloat(entity.pos.z, hold_posz);
        ptr += sizeof(uint32_t);

        uint32_t hold_yaw;
        memcpy(&hold_yaw, ptr, sizeof(uint32_t));
        convertToFloat(entity.yaw, hold_yaw);
        ptr += sizeof(uint32_t);

        uint32_t hold_pitch;
        memcpy(&entity.pitch, ptr, sizeof(uint32_t));
        convertToFloat(entity.pitch, hold_pitch);
        ptr += sizeof(uint32_t);

        memcpy(&entity.name, ptr, sizeof(uint8_t) * 64);

        data.entities.push_back(entity);
    }

    void Client::receiveRemoveEntity()
    {
        //entityID[int]
        receiveAll(4);
    }

    void Client::receiveUpdateEntity()
    {
        //entityID[int], xpos[float], ypos[float], zpos[float], yaw[float], pitch[float]
        receiveAll(24);
        EntityData entity;
        uint8_t *ptr = &buffer[0];

        memcpy(&entity.id, ptr, sizeof(int));
        // std::cout << "ID without conversion: " << entity.id << std::endl;
        entity.id = be32toh(entity.id);
        ptr += sizeof(int);

        uint32_t hold_posx;
        memcpy(&hold_posx, ptr, sizeof(uint32_t));
        convertToFloat(entity.pos.x, hold_posx);
        ptr += sizeof(uint32_t);

        uint32_t hold_posy;
        memcpy(&hold_posy, ptr, sizeof(uint32_t));
        convertToFloat(entity.pos.y, hold_posy);
        ptr += sizeof(uint32_t);

        uint32_t hold_posz;
        memcpy(&hold_posz, ptr, sizeof(uint32_t));
        convertToFloat(entity.pos.z, hold_posz);
        ptr += sizeof(uint32_t);

        uint32_t hold_yaw;
        memcpy(&hold_yaw, ptr, sizeof(uint32_t));
        convertToFloat(entity.yaw, hold_yaw);
        ptr += sizeof(uint32_t);

        uint32_t hold_pitch;
        memcpy(&entity.pitch, ptr, sizeof(uint32_t));
        convertToFloat(entity.pitch, hold_pitch);

        data.entities.push_back(entity);
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

        chunk.blocktypes.resize(4096);
        memcpy(&chunk.blocktypes[0], ptr, 4096);

        mtx_chunk_data.lock();
        data.chunks.push_back(chunk); // to lock
        mtx_chunk_data.unlock();
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

        memcpy(&blocktype, ptr, sizeof(uint8_t));
        chunk.blocktypes.resize(4096);
        std::fill(std::begin(chunk.blocktypes), std::end(chunk.blocktypes), blocktype);

        mtx_chunk_data.lock();
        data.chunks.push_back(chunk);
        mtx_chunk_data.unlock();
    }

    void Client::receiveChat()
    {
        receiveAll(4096);
    }

    void Client::receiveMetaData()
    {
        receiveAll(68);
    }

    void Client::sendUpdateEntity(float xpos, float ypos, float zpos, float yaw, float pitch)
    {
        //id[byte], entityID[int], xpos[float], ypos[float], zpos[float], yaw[float], pitch[float], name[char 64]
        uint8_t id = 0x00;

        ued.id = id;
        ued.xpos = htobe32(*(uint32_t*)&xpos);
        ued.ypos = htobe32(*(uint32_t*)&ypos);
        ued.zpos = htobe32(*(uint32_t*)&zpos);
        ued.yaw = htobe32(*(uint32_t*)&yaw);
        ued.pitch = htobe32(*(uint32_t*)&pitch);

        send(client_socket, &ued, sizeof(ued), 0);
    }

    void Client::sendUpdateBlock(uint8_t block_type, int xpos, int ypos, int zpos)
    {
        uint8_t id = 0x01;

        ubd.id = id;
        ubd.blocktype = block_type;
        ubd.xpos = htobe32(*(uint32_t*)&xpos);
        ubd.ypos = htobe32(*(uint32_t*)&ypos);
        ubd.zpos = htobe32(*(uint32_t*)&zpos);

        send(client_socket, &ubd, sizeof(ubd), 0);
    }

    void Client::sendRenderDistance(uint8_t distance)
    {
        uint8_t id = 0x04;

        umd.id = id;
        umd.render_distance = distance;

        memcpy(umd.name, &name, sizeof(umd.name));
        umd.name[sizeof(umd.name) - 1] = '\0';

        send(client_socket, &umd, sizeof(umd), 0);
    }
}
