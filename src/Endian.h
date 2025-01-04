#pragma once

#include <cstring>

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h> // Windows Sockets library for htonl, ntohl, etc.
    #define htobe32(x) htonl(x)
    #define be32toh(x) ntohl(x)
    #define htobe16(x) htons(x)
    #define be16toh(x) ntohs(x)
#else
    #include <arpa/inet.h> // For htonl, ntohl, etc., on Linux
    #include <endian.h>    // For htobe32, be32toh, etc.
#endif