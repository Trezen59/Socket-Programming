// UDP server code for chatting application 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX 100
#define PORT 8080

// func to chat 

void chat_func(struct sockaddr_in serv_addr, struct sockaddr_in cli_addr, int sockfd)
{
	char buff[MAX];
	char buff2[MAX];
	
	int l, len, n;
	
	
	// infinite loop for chat
	
	while(1) {
	
		len = sizeof(cli_addr);
		
		bzero(buff, MAX);	// erases the given mem with zero bytes
		
		n = recvfrom(sockfd, (char *)buff, MAX, MSG_WAITALL, (struct sockaddr *) &cli_addr, &len);
		
		buff[n] = '\0';
		
		printf("client : %s\n", buff);
		
		bzero(buff2, MAX);
		
		l = 0;
		
		printf("msg : ");
		
		while((buff2[l++] = getchar()) != '\n')
			;
			
		
		sendto(sockfd, (const char *)buff2, strlen(buff2), MSG_CONFIRM, (const struct sockaddr *) &cli_addr, len);
		buff2[l] = '\0';
		perror("sendto");
		printf("message sent\n\n");

		// if msg is "bye", exit the application
		
		if (!strncmp("bye", buff2, 3)) {
			printf("Server stopped ...\n");
			break;
		}
	}
}


int main()
{
	int sockfd, connfd, len;
	
	struct sockaddr_in serv_addr, cli_addr;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
		
	bzero(&serv_addr, sizeof(serv_addr));
	bzero(&cli_addr, sizeof(cli_addr));

	// assign IP, PORT
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// long integer from host byte order to network byte order.
	serv_addr.sin_port = htons(PORT);	// short integer from host byte order to network byte order.

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Function to chat between client and server
	chat_func(serv_addr, cli_addr, sockfd);

	// After chatting close the socket
	close(sockfd);
}

