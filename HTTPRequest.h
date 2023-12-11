#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

enum HTTPMethods
{
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    OPTIONS,
    TRACE,
    CONNECT,
    PATCH
};

struct HTTPRequest
{
    int method;
    char *uri;
    float http_version;
};

struct HTTPRequest http_request_constructor(char *request_string);


#endif