#include "HTTPRequest.h"
#include <string.h>
#include <stdlib.h>
#include "c_data_structures/Queue.h"
#include <stdbool.h>
#include <stdio.h>

void extract_request_line(HTTPRequest *request, char *request_line);
void extract_header_fields(HTTPRequest *request, char *header_fields);
void extract_body(HTTPRequest *request, char *body);
void parse_url_params(HTTPRequest *request, char *uri);

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

HTTPRequest http_request_constructor(char *request_string)
{
    // copy beacuse request_string is immutable
    char temp_request_string[strlen(request_string) + 1];
    strcpy(temp_request_string, request_string);
    temp_request_string[strlen(request_string)] = '\0';

    // replace \n\n with | to use strtok, iterate for length -1 because we check the next character
    // There is some weird character in the middle of \n and \n
    for (int i = 0; i < strlen(temp_request_string) - 2; i++)
    {
        if (temp_request_string[i] == '\n' && temp_request_string[i + 2] == '\n')
        {
            temp_request_string[i + 2] = '|';
        }
    }

    char *request_line = strtok(temp_request_string, "\n");
    char *header_fields = strtok(NULL, "|");
    char *body = strtok(NULL, "\0");

    HTTPRequest request;
    request.url_params = dictionary_constructor(COMPARE_STR_KEYS);
    request.request_line = dictionary_constructor(COMPARE_STR_KEYS);
    request.headers = dictionary_constructor(COMPARE_STR_KEYS);

    extract_request_line(&request, request_line);
    extract_header_fields(&request, header_fields);
    extract_body(&request, body);

    if (strcmp(request.request_line.search(&request.request_line, "method", sizeof("method")), "GET") == 0)
    {
        parse_url_params(&request, request.request_line.search(&request.request_line, "uri", sizeof("uri")));
    }

    return request;
}

void extract_request_line(HTTPRequest *request, char *request_line)
{
    char fields[strlen(request_line)];
    strcpy(fields, request_line);
    char *method = strtok(fields, " ");
    char *uri = strtok(NULL, " ");
    char *http_version = strtok(NULL, "\0");

    request->request_line.insert(&request->request_line, "method", sizeof("method"), method, sizeof(char[strlen(method)]));
    request->request_line.insert(&request->request_line, "uri", sizeof("uri"), uri, sizeof(char[strlen(uri)]));
    request->request_line.insert(&request->request_line, "http_version", sizeof("http_version"), http_version, sizeof(char[strlen(http_version)]));
}

void extract_header_fields(HTTPRequest *request, char *header_fields)
{

    char fields[strlen(header_fields)];
    strcpy(fields, header_fields);
    Queue header_fields_queue = queue_constructor(false, 0);

    char *header_field = strtok(fields, "\n");

    while (header_field != NULL)
    {
        header_fields_queue.enqueue(header_field, sizeof(char[strlen(header_field)]), &header_fields_queue);
        header_field = strtok(NULL, "\n");
    }

    char *header_item = (char *)header_fields_queue.peek(&header_fields_queue);

    while (strlen(header_item) > 3 && header_item != NULL)
    {
        char *header_key = strtok(header_item, ":");
        char *header_value = strtok(NULL, "\0");

        request->headers.insert(&request->headers, header_key, sizeof(char[strlen(header_key)]), header_value, sizeof(char[strlen(header_value)]));

        header_fields_queue.dequeue(&header_fields_queue);
        header_item = (char *)header_fields_queue.peek(&header_fields_queue);
    }
}

void extract_body(HTTPRequest *request, char *body)
{
    if (body == NULL)
    {
        return;
    }

    char body_text[strlen(body)];
    strcpy(body_text, body);

    request->body = body;
}

void parse_url_params(HTTPRequest *request, char *uri)
{

    char fields[strlen(uri)];
    strcpy(fields, uri);

    char *url_params = strtok(fields, "?");

    if (url_params == NULL)
    {
        return;
    }

    url_params = strtok(NULL, "\0");

    if (url_params == NULL)
    {
        return;
    }

    Queue url_params_queue = queue_constructor(false, 0);

    char *url_param = strtok(url_params, "&");

    if (url_param == NULL)
    {
        return;
    }

    while (url_param != NULL)
    {
        url_params_queue.enqueue(url_param, sizeof(char[strlen(url_param)]), &url_params_queue);
        url_param = strtok(NULL, "&");
    }

    char *url_param_item = (char *)url_params_queue.peek(&url_params_queue);

    while (url_param_item != NULL)
    {
        char *url_param_key = strtok(url_param_item, "=");
        char *url_param_value = strtok(NULL, "\0");

        request->url_params.insert(&request->url_params, url_param_key, sizeof(char[strlen(url_param_key)]), url_param_value, sizeof(char[strlen(url_param_value)]));

        url_params_queue.dequeue(&url_params_queue);
        url_param_item = (char *)url_params_queue.peek(&url_params_queue);
    }
}