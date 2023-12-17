#include "HTTPServer.h"
#include <stdio.h>

char *home(HTTPServer *server, HTTPRequest *request)
{
    char *template = render_template(2, "/home/ravin/projects/webserver/templates/home1.html", "/home/ravin/projects/webserver/templates/home2.html");
    return template;
}

int main()
{
    HTTPServer server = http_server_constructor();
    server.register_routes(&server, home, "/", 1, GET);
    server.launch(&server);
}