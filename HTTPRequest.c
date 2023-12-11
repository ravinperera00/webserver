#include "HTTPRequest.h"
#include <string.h>
#include <stdlib.h>

int method_select(char *method)
{
    if (strcmp(method, "GET") == 0)
    {
        return GET;
    }
    else if (strcmp(method, "POST") == 0)
    {
        return POST;
    }
    else if (strcmp(method, "PUT") == 0)
    {
        return PUT;
    }
    else if (strcmp(method, "HEAD") == 0)
    {
        return HEAD;
    }
    else if (strcmp(method, "PATCH") == 0)
    {
        return PATCH;
    }
    else if (strcmp(method, "DELETE") == 0)
    {
        return DELETE;
    }
    else if (strcmp(method, "CONNECT") == 0)
    {
        return CONNECT;
    }
    else if (strcmp(method, "OPTIONS") == 0)
    {
        return OPTIONS;
    }
    else if (strcmp(method, "TRACE") == 0)
    {
        return TRACE;
    }
    else
    {
        return -1;
    }
}

struct HTTPRequest http_request_constructor(char *request_string)
{
    // copy beacuse request_string is immutable
    char temp_request_string[strlen(request_string)];
    strcpy(temp_request_string, request_string);

    // replace \n\n with | to use strtok, iterate for length -1 because we check the next character
    for (int i = 0; i < strlen(temp_request_string) - 1; i++)
    {
        if (temp_request_string[i] == '\n' && temp_request_string[i + 1] == '\n')
        {
            temp_request_string[i + 1] = '|';
        }
    }

    char *request_line = strtok(temp_request_string, "\n");
    char *header_fields = strtok(NULL, "|");
    char *body = strtok(NULL, "|");

    struct HTTPRequest request;

    char *method = strtok(request_line, " ");

    request.method = method_select(method);

    char *uri = strtok(NULL, " ");

    request.uri = uri;

    char *http_version = strtok(NULL, " ");
    http_version = strtok(http_version, "/");
    http_version = strtok(NULL, "/");

    request.http_version = (float)atof(http_version);

    return request;
}