#include "Server.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "HTTPRequest.h"

void launch(struct Server *server)
{
    char buffer[30000];
    printf("======== WAITING FOR CONNECTION ========\n");
    int address_length = sizeof(server->address);
    int new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_length);
    read(new_socket, buffer, 30000);
    printf("%s\n", buffer);
    struct HTTPRequest request = http_request_constructor(buffer);
    printf("HTTP Version: %f\n", request.http_version);
    char *default_response = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Hello, World!</h1></body></html>";
    write(new_socket, default_response, strlen(default_response));
    close(new_socket);
}

int main()
{
    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 10, launch);
    server.launch(&server);
    return 0;
}