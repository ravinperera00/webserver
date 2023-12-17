#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

typedef struct Server
{
    int domain;
    int service;
    int protocol;
    u_long interface;
    int port;
    int backlog;
    int socket;

    struct sockaddr_in address;
} Server;

Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog);

#endif