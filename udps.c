// UDP server code for chatting application 

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

void chat_func(int sockfd, struct sockaddr_in serv_addr, struct sockaddr_in cli_addr)
{
	char buff[MAX];
	int flag = 1;
	socklen_t len;
	int *length;

	length =(int *)malloc(sizeof(int));
	
	if(length == NULL)
	{
		printf("failed to allocate memory..\n");
		close(sockfd);
		exit(0);
	}

	printf("\n************ UDP Server *************\n\n");
	
	printf("Chat rules : \n 1) Send an empty message when you are done sending messages.\n 2) send 'bye' to exit the chat.\n\n ");

	while(1) 
	{
		while(flag == 1)
		{
			memset(buff, '\0', MAX);
			
			memset(length, '\0', sizeof(length));

			// recieve length and store it in length buffer
			if((recvfrom(sockfd, length, sizeof(length), MSG_WAITALL, (struct sockaddr *)&serv_addr, &len)) == -1)
			{
				perror("recvfrom ");
				printf("failed to read length\n");
			}

			// check for length of msg
			if(*length > MAX)
			{
				printf("the length of data recieved = %d\n", *length);
			}
			*length = 0;

			// recieve data and store in buff
			if((recvfrom(sockfd, (char *) buff, MAX, MSG_WAITALL, (struct sockaddr *) &serv_addr, &len)) == -1)
			{
				perror("recvfrom ");
				printf("failed to read data\n");
				free(length);
				close(sockfd);
				exit(0);
			}
			
			// if buff is empty change mode to write
			if (strlen(buff) == 1)
			{
				flag = 0;
				break;
			}
			

			// if buff contains bye exit the chat
			if(! strncmp("bye", buff, 3))
			{
				printf("Chat exited..!\n");
				free(length);
				close(sockfd);
				exit(0);
			}
			
			// print the msg
			printf("\nClient: %s", buff);
		}

		while(flag == 0)
		{
			memset(buff, '\0', MAX);	
			memset(length, '\0', sizeof(length));

			printf("\nServer : ");
			
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
			
			// send the lenght of msg stored in buff
			if((sendto(sockfd, length, sizeof(length), MSG_CONFIRM, (const struct sockaddr *)&serv_addr, sizeof(serv_addr))) == -1)
			{
				printf("failed to write length\n");
			}
			*length = 0;

			// send the msg stored in buffer		
			if((sendto(sockfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &serv_addr, sizeof(serv_addr))) == -1)
			{
				printf("failed to write data");
				free(length);
				close(sockfd);
				exit(0);
			}
			
			if(strlen(buff) == 1)
			{
				flag = 1;
			}
			
			if(! strncmp("bye", buff, 3))
			{
				printf("Chat exited..!\n");
				free(length);
				close(sockfd);
				exit(0);
			}



		}// while end write			
	}
}


int main()
{
	int sockfd;
	
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
	chat_func(sockfd, serv_addr, cli_addr);

	// After chatting close the socket
	close(sockfd);
}

