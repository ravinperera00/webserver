#include "ThreadPool.h"
#include "HTTPServer.h"
#include "Server.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "HTTPRequest.h"
#include "c_data_structures/Dictionary.h"
#include "Route.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void *handler(void *arg);
void launch(HTTPServer *server);
void register_routes(struct HTTPServer *server, char *(*route_function)(struct HTTPServer *server, struct HTTPRequest *request), char *uri, int num_methods, ...);

HTTPServer http_server_constructor()
{
    struct HTTPServer server;
    server.server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 255);
    server.routes = dictionary_constructor(COMPARE_STR_KEYS);
    server.register_routes = register_routes;
    server.launch = launch;
    return server;
}

void register_routes(struct HTTPServer *server, char *(*handler)(struct HTTPServer *server, struct HTTPRequest *request), char *uri, int num_methods, ...)
{
    struct Route route;
    va_list methods;
    va_start(methods, num_methods);
    for (int i = 0; i < num_methods; i++)
    {
        route.methods[i] = va_arg(methods, int);
    }
    char buffer[strlen(uri)];
    route.uri = buffer;
    strcpy(route.uri, uri);
    route.handler = handler;
    server->routes.insert(&server->routes, uri, sizeof(char[strlen(uri)]), &route, sizeof(route));
}

void launch(HTTPServer *server)
{
    ThreadPool *thread_pool = thread_pool_constructor(10);
    struct sockaddr *sock_addr = (struct sockaddr *)&server->server.address;
    socklen_t *address_length = malloc(sizeof(socklen_t));
    if (address_length == NULL)
    {
        perror("Failed to allocate memory for address_length");
        exit(1);
    }
    *address_length = (socklen_t)sizeof(server->server.address);
    while (true)
    {
        ClientServer *client_server = (ClientServer *)malloc(sizeof(ClientServer));
        if (client_server == NULL)
        {
            perror("Failed to allocate memory for client_server");
            exit(1);
        }
        printf("Waiting for client connection...\n");
        int client_socket = accept(server->server.socket, sock_addr, address_length);
        if (client_socket == -1)
        {
            perror("Failed to accept client connection");
            free(client_server);
            exit(1);
        }
        client_server->client = client_socket;
        client_server->server = server;
        Task *task = task_constructor(handler, client_server);
        thread_pool->add_task(thread_pool, task);
    }
}

void *handler(void *arg)
{
    ClientServer *client_server = (ClientServer *)arg;
    char request_string[30000];
    read(client_server->client, request_string, 30000);
    HTTPRequest request = http_request_constructor(request_string);
    printf("%s\n", request_string);
    char *uri = request.request_line.search(&request.request_line, "uri", sizeof("uri"));
    Route *route = client_server->server->routes.search(&client_server->server->routes, uri, sizeof(char[strlen(uri)]));
    char *response;
    if (route == NULL)
    {
        response = "HTTP/1.1 404 Not Found\nContent-Type: text/html\n\n<html><body><h1>404 Not Found</h1></body></html>";
    }
    else
    {
        response = route->handler(client_server->server, &request);
    }
    // char *default_response = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Hello, World!</h1></body></html>";
    write(client_server->client, response, sizeof(char[strlen(response)]));
    close(client_server->client);
    free(client_server);
    return NULL;
}

char *render_template(int num_templates, ...)
{
    char *buffer = malloc(30000);
    int buffer_position = 0;
    char c;
    FILE *file;
    va_list files;
    va_start(files, num_templates);
    for (int i = 0; i < num_templates; i++)
    {
        char *path = va_arg(files, char *);
        file = fopen(path, "r");
        while ((c = fgetc(file)) != EOF)
        {
            buffer[buffer_position] = c;
            buffer_position += 1;
        }
    }
    va_end(files);
    return buffer;
}