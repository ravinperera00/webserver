# Simple Webserver

This is a simple library that allows you to quickly create a very simple HTTP webserver.

## Building the project

 1. Clone the repository to your desired location using the `git clone --recurse-submodules <repository_url>` command.
 2. Run the `build.sh` script to build the library including the necessary dependencies.
 4. Once done with your project, clean the project by running the `clean.sh` script.

## How to use the library

 1. Include the `HTTPServer.h` file to your project.
 2. Create a server using the `http_server_constructor`.
 3. Add the routes you need to the server by calling the `register_routes` method in the server structure.
 4. Run the `launch` method in the server structure.

An example is given below.

    #include "HTTPServer.h"
    #include <stdio.h>

    char  *home(HTTPServer  *server,  HTTPRequest  *request)
    {
	    int number_of_files = 2
	    char  *template  =  render_template(number_of_files, "path1", "path2");
	    return template;
	}

	int main()
	{
		HTTPServer  server = http_server_constructor();
		server.register_routes(&server, home, "/", 1, GET);
		server.launch(&server);
		return 0;
	}


## Notes
The project uses a custom library of data structures written in C. [Link](https://github.com/ravinperera00/c_data_structures)

The library is extremely primitive and only appropriate for educational projects and not for any commercial use.

Feel free to provide feedback on what I can improve and issue you face 🙌🏽. 