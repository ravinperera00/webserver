#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "c_data_structures/Dictionary.h"

typedef struct HTTPRequest
{
    Dictionary request_line;
    Dictionary headers;
    char *body;
    Dictionary url_params;
} HTTPRequest;

HTTPRequest http_request_constructor(char *request_string);

#endif