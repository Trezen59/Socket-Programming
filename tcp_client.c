// TCP client code for chatting application 

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define MAX 100
#define PORT 8080

// function to chat between client and server

void chat_func(int sockfd)
{
	char buff[MAX];	// buff to enter the message that we need to send
	int n;
	
	while(1)
	{
		// clear the buffer "buff"
		bzero(buff, sizeof(buff));
		
		printf("\nEnter msg : ");
		n = 0;
		
		// take input in buff
		while((buff[n++] = getchar()) != '\n')
			;
		
		// write to buffer pointed by sockfd from buff 
		write(sockfd, buff, sizeof(buff));
		
		// clear the buffer 
		bzero(buff, sizeof(buff));
		
		// get data from buffer pointed by sockfd
		read(sockfd, buff, sizeof(buff));
		
		printf("\nFrom Server : %s", buff);
		
		// enter "bye" to exit the chat
		if ((strncmp(buff, "bye", 3)) == 0) {
			printf("\nChat Exited...\n");
			break;
		}
	}
}

int main()
{
	int sockfd, connfd;
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
	// enter IP of server to connect
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // currently on loopback
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) 
	{
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	// function to chat
	chat_func(sockfd);

	// close the socket
	close(sockfd);
}

