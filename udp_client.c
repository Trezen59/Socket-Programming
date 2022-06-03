// UDP client code for chatting application 

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

void chat_func(struct sockaddr_in serv_addr, int sockfd)
{
	char buff[MAX];
	char buff2[MAX];
	
	int l, len, n;
	
	// infinite loop for chat
	
	while(1) {
			
		bzero(buff, MAX);	// erases the given mem with zero bytes
		
		l = 0;
		
		printf("\nmsg : ");
		
		while((buff[l++] = getchar()) != '\n')
			;
			
		// send msg to server
		sendto(sockfd, (const char *) buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof(serv_addr));
		buff[l] = '\0';
		
		perror("sendto");
		
		// recieve msg from server
		n = recvfrom(sockfd, (char *) buff2, MAX, MSG_WAITALL, (struct sockaddr *) &serv_addr, &len);
		buff2[n] = '\0';
		
		printf("\nserver : %s\n", buff2);
		
		// to end the chat send "bye"		
		if (!strncmp("bye", buff, 3)) {
			printf("Client stopped ...\n");
			break;
		}
	}
}


int main()
{
	int sockfd, len;
	
	struct sockaddr_in serv_addr ;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
		
	bzero(&serv_addr, sizeof(serv_addr));

	// assign IP, PORT
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	// long integer from host byte order to network byte order.
	serv_addr.sin_port = htons(PORT);	// short integer from host byte order to network byte order.


	// Function to chat between client and server
	chat_func(serv_addr, sockfd);

	// After chatting close the socket
	close(sockfd);
}

