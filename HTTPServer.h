#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "Server.h"
#include "c_data_structures/Dictionary.h"
#include "HTTPRequest.h"

typedef struct HTTPServer
{
    Server server;
    Dictionary routes;
    void (*register_routes)(struct HTTPServer *server, char *(*handler)(struct HTTPServer *server, HTTPRequest *request), char *uri, int num_methods, ...);
    void (*launch)(struct HTTPServer *server);
} HTTPServer;

typedef struct ClientServer
{
    int client;
    HTTPServer *server;
} ClientServer;

enum HTTPMethods
{
    CONNECT,
    DELETE,
    GET,
    HEAD,
    OPTIONS,
    PATCH,
    POST,
    PUT,
    TRACE
};

HTTPServer http_server_constructor(void);

char *render_template(int num_templates, ...);

#endif