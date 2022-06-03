// TCP server code for chatting application 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 100
#define PORT 8080

// func to chat between client and server

void chat_func(int connfd)
{
	char buff[MAX];
	int n;
	
	// infinite loop for chat
	
	while(1) {
	
		bzero(buff, MAX);	// erases the given mem with zero bytes

		// read the message from client and copy it in buffer
		
		read(connfd, buff, sizeof(buff));
		
		// print buffer which contains the client contents
		
		printf("\nFrom client: %s\nTo client : ", buff);
		
		bzero(buff, MAX);
		n = 0;
		
		// copy server message in the buffer
		
		while ((buff[n++] = getchar()) != '\n')
			;

		// send that buffer to client
		
		write(connfd, buff, sizeof(buff));

		// if msg is "bye", exit the application
		
		if (!strncmp("bye", buff, 3)) {
			printf("Server stopped ...\n");
			break;
		}
	}
}


int main()
{
	int sockfd, connfd, len;
	
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
		
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	// long integer from host byte order to network byte order.
	servaddr.sin_port = htons(PORT);	// short integer from host byte order to network byte order.

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// server listens
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
		
	len = sizeof(cli);

	// Accept the data from client
	connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
	
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else
		printf("server accepted the client...\n");

	// Function to chat between client and server
	chat_func(connfd);

	// After chatting close the socket
	close(sockfd);
}

