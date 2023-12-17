all: Networking

Networking: NetworkingSub
	ar rcs libwebserver.a Server.o HTTPRequest.o HTTPServer.o c_data_structures/Node.o c_data_structures/LinkedList.o c_data_structures/Queue.o c_data_structures/BinarySearchTree.o c_data_structures/Entry.o c_data_structures/Dictionary.o ThreadPool.o

NetworkingSub: Server HTTPRequest HTTPServer ThreadPool

Server:
	gcc -c Server.c

HTTPServer:
	gcc -c HTTPServer.c

HTTPRequest:
	gcc -c HTTPRequest.c

ThreadPool:
	gcc -c ThreadPool.c

clean:
	rm -rf *.o *.a