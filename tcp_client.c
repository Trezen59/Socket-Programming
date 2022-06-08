// TCP client code for chatting application 

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define MAX 100
#define PORT 8080

// function to chat between client and server

void chat_func(int sockfd)
{
	char buff[MAX];	// buff to enter the message that we need to send
	int flag = 0;

	memset(buff, '\0', MAX);	
	printf("\n************ Client *************\n");
		
	while(1)
	{
		while(flag == 0)
		{
			memset(buff, '\0', MAX);	
			
			printf("\nClient : ");
			
			int i=0;
			while((buff[i++] = getchar()) != '\n')
				;

			if((write(sockfd, buff, sizeof(buff))) == -1)
			{
				printf("failed to write data");
				close(sockfd);
				exit(0);
			}
			
			if(strlen(buff) == 1)
			{
				flag = 1;
			}

		}// while end write
		
		while(flag == 1)
		{
			memset(buff, '\0', MAX);

			if((read(sockfd, buff, sizeof(buff))) == -1)
			{
				printf("failed to read data\n");
				close(sockfd);
				exit(0);
			}
						
			if (strlen(buff) == 1)
			{
				flag = 0;
				break;
			}

			printf("\nServer : %s", buff);
		}
	}
}

int main()
{
	int sockfd;
	struct sockaddr_in servaddr;

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
