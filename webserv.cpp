/*
	int socket(int domain, int type, int protocol);
		- returns FD used for communication
		- -1 on error

	int bind(int sockfd, const sockaddr *addr, socklen_t addrlen);
		- Assign IP address and port to socket
		- -1 on error, else 0

	int listen(int sockfd, int backlog);
		- Marks socket as passive (awaiting to accept connection)
		- -1 on error, else 0
		- backlog: we can create a queue of connections, up to "backlog" number

	int accept(int sockfd, sockaddr *addr, socklen_t *addrlen);
		- Accepts connection from listen queue
		- returns FD, or -1 on error

	struct sockaddr_in 
	{
		sa_family_t    sin_family; address family: AF_INET 
		in_port_t      sin_port;   port in network byte order 
		struct in_addr sin_addr;   internet address 
	}
		- Predefined structure to store socket address (IP + PORT)

	htonl converts ADDRESS to a network representation 
	htons converts PORT to a network representation 
*/

#include <iostream>

// either one for htons, htonl
#include <netinet/in.h> 
// #include <arpa/inet.h> 

#include <sys/socket.h>
#include <unistd.h> // read

// TESTING : nc -n -v 127.0.0.1 9999 < <(echo "hello world")
int main()
{
	// Create a socket (IPv4, TCP)
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) 
	{
    	std::cout << "Failed to create socket. errno: " << errno << std::endl;
    	exit(EXIT_FAILURE);
  	}

	sockaddr_in sockaddr;
	sockaddr.sin_family = AF_INET;
  	sockaddr.sin_addr.s_addr = INADDR_ANY; // IP ADDRESS (Any available)
	sockaddr.sin_port = htons(9999); // PORT

	// Assign IP address + port to socket
	if (bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) // < 0, replaced by !
	{
		std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
  	}

	// Loop in actual code ?
	if (listen(sockfd, 10) < 0) 
	{
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
  	}


	int addrlen = sizeof(sockaddr);
	int connection = accept(sockfd, (struct sockaddr *) &sockaddr, (socklen_t *) &addrlen);
	if (connection < 0) 
	{
		std::cout << "Failed to grab connection. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	// Read from the connection
	char buffer[100];
	int bytesRead = read(connection, buffer, 100);
	std::cout << "The message was: " << buffer;

	// Send a message to the connection
	std::string response = "Good talking to you\n";
	send(connection, response.c_str(), response.size(), 0);

	// Close the connections
	close(connection);
	close(sockfd);

	return (0);
}