#ifndef ROUTE_H
#define ROUTE_H

#include "HTTPRequest.h"
#include "HTTPServer.h"

typedef struct Route
{
    int methods[9];
    char *uri;
    char *(*handler)(HTTPServer *server, HTTPRequest *request);
} Route;

#endif