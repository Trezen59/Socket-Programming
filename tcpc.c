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
	int flag = 0;
	int infi_flag = 1;

	char buff[MAX];	// buff to enter the message that we need to send
	int *len;

	len = (int *)malloc(sizeof(int));
	
	if(len == NULL)
	{
		printf("failed to allocate memory..\n");
		close(sockfd);
		exit(0);
	}

	memset(buff, '\0', sizeof(buff));	
	printf("\n\n\n************ TCP Client *************\n");
	
	printf("Chat rules : \n 1) Send an empty message when you are done sending messages.\n 2) send 'bye' to exit the chat.\n\n ");


	while(infi_flag)
	{
		while(flag == 0)
		{
			
			memset(buff, '\0', sizeof(buff)); 
			memset(len, '\0', sizeof(len));

			printf("\nClient : ");
			
			int bufflen;	
			
			// take input into buffer
			int i=0;
			while((buff[i++] = getchar()) != '\n')
				;
	
			buff[i]	= '\0';		
			
			// claculate and print len of message in buffer
			bufflen = i-1;			
			//printf("bufflen =  %d\n", bufflen);
			

			// if bufflen is more than buffer size print error msg and clear buffer to write again
			if(bufflen > MAX)
			{
				printf("msg size exceeded..! \n");
				memset(buff, '\0', MAX);
			}
			
			// send the length of msg in buffer

			*len = bufflen;

			if((write(sockfd, len, sizeof(len))) == -1)
			{	
				printf("failed to write length\n");
			}
			*len = 0;

			// send the msg which is stored in buffer 
			if((write(sockfd, buff, sizeof(buff))) == -1)
			{
				printf("failed to write data");
				free(len);
				close(sockfd);
				//infi_flag = 0;
				exit(0);
				//printf("exiting..\n");
				
			}
			
			// if buff is empty then switch to read mode by setting flag to 1
			if(strlen(buff) == 1)
			{
				flag = 1;
				break;
			}

			// if we write bye to buff the chat is ended
			if(! strncmp("bye", buff, 3))
			{
				printf("Chat exited..!\n");
				close(sockfd);
				free(len);
				exit(0);
			}


		}// while end write
		
		while(flag == 1)
		{
			memset(buff, '\0', MAX);
			
			// read recieved length
			
			if((read(sockfd, len, sizeof(len))) == -1)	
			{
				printf("failed to read the length \n");
			}
			
			if(*len < MAX)
			{
				printf("the length of data recieved = %d\n", len[0]);
			}
			*len = 0;

			// read buffer data
			if((read(sockfd, buff, MAX)) == -1)
			{
				printf("failed to read data\n");
				free(len);
				close(sockfd);
				//infi_flag = 0;
				exit(0);
				//printf("exiting..\n");
			}

			printf("Server : %s\n", buff);
			
			// if buffer is empty then switch to write mode by setting flag to 0
			if (strlen(buff) == 1)
			{
				flag = 0;
				break;
			}
			
			if(! strncmp("bye", buff, 3))
			{
				printf("Chat exited..!\n");
				close(sockfd);
				free(len);
				exit(0);
			}

		}

	}
	free(len);
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
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) 
	{
		printf("connection with the server failed...\n");
		close(sockfd);
		exit(0);
	}
	else
		printf("connected to the server..\n");
		// function to chat
		chat_func(sockfd);

	// close the socket
	close(sockfd);

	return 0;
}

