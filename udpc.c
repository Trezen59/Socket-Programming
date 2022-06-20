// UDP client code for chatting application 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 100
#define PORT 8080

// func to chat

void chat_func(int sockfd, struct sockaddr_in serv_addr)
{
	char buff[MAX];	// buff to enter the message that we need to send
	int flag = 0;
	socklen_t len;
	int *length;

	length = (int *)malloc(sizeof(int));
	
	if(length == NULL)
	{
		printf("failed to allocate memory..\n");
		close(sockfd);
		exit(0);
	}

	memset(buff, '\0', MAX);	
	printf("\n\n************ UDP Client *************\n\n");

	printf("Chat rules : \n 1) Send an empty message when you are done sending messages.\n 2) send 'bye' to exit the chat.\n\n ");

	while(1)
	{
		while(flag == 0)
		{
			memset(buff, '\0', MAX);	
			
			memset(length, '\0', sizeof(length));	
			printf("\nClient : ");
			
			int bufflen;
			int i=0;
			
			// take input in buff 
			while((buff[i++] = getchar()) != '\n')
				;
			bufflen = i-1;
			
			// check for max length of msg
			if(bufflen > MAX)
			{
				printf("msg limit exceeded..!\n");
				memset(buff, '\0', MAX);
			}
			
			//int length[1];
			*length = bufflen;

			// send the length of msg stored in buff
			if((sendto(sockfd, length, sizeof(length), MSG_CONFIRM, (const struct sockaddr *)&serv_addr, sizeof(serv_addr))) == -1)
			{
				printf("failed to write length\n");
			}
			*length = 0;

			// send the msg stored in buff
			if((sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof(serv_addr))) == -1)
			{
				printf("failed to write data");
				free(length);
				close(sockfd);
				exit(0);
			}
			
			// if the buff is empty change mode to write
			if(strlen(buff) == 1)
			{
				flag = 1;
			}

			// if buff contains bye exit the chat
			if(! strncmp("bye", buff, 3))
			{
				printf("Chat exited..!\n");
				free(length);
				close(sockfd);
				exit(0);
			}


		}// while end write
		
		while(flag == 1)
		{
			memset(buff, '\0', MAX);
			//int length[1];

			memset(length, '\0', sizeof(length));

			// recieve the length of msg
			if((recvfrom(sockfd, length, sizeof(length), MSG_WAITALL, (struct sockaddr *)&serv_addr, &len)) == -1)
			{
				perror("recvfrom ");
				printf("failed to read length\n");
			}

			// check for max length permitted
			if(*length > MAX)
			{
				printf("the length of data recieved = %d\n", length[0]);
			}
			*length = 0;

			// recieve the msg and store it in buff
			if((recvfrom(sockfd, (char *) buff, MAX, MSG_WAITALL, (struct sockaddr *) &serv_addr, &len)) == -1)
			{
				perror("recvfrom ");
				printf("failed to read data\n");
				free(length);
				close(sockfd);
				exit(0);
			}
						
			if (strlen(buff) == 1)
			{
				flag = 0;
				break;
			}
			
			if(! strncmp("bye", buff, 3))
			{
				printf("Chat exited..!\n");
				free(length);
				close(sockfd);
				exit(0);
			}

			printf("\nServer : %s", buff);
		}
	}
}

int main()
{
	int sockfd;
	
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
	chat_func(sockfd, serv_addr);

	// After chatting close the socket
	close(sockfd);
}

