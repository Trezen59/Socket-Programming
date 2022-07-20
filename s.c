// server app

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAX 100
#define PORT1 8080

int thread1, thread2;
pthread_t t1, t2;

int create_socket()
{
	int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		close(sockfd);
		exit(0);
	}
	else
	{
		printf("Socket successfully created..\n");
		return sockfd;
	}
}


void *reading(void *connfd)
{
	int flag = 1;

	// declare bufflen to store length of msg
	int *bufflen = malloc(sizeof(int));
	if(bufflen == NULL)
	{
		printf("failed to allocate memory - bufflen - func reading \n");
		close(*(int *)connfd);
		exit(0);

	}
	bzero(bufflen, sizeof(int));

	while(flag)
	{

		bzero(bufflen, sizeof(int));		

		// read length of buffer
		if((read(*(int *)connfd, bufflen, sizeof(int))) == -1)
		{
			perror("read");
			//free(bufflen);
			//exit(0);
			break;
		}	
		printf("recieved length : %d\n", *bufflen);

		char *buff = malloc(*bufflen);
		if(buff == NULL)
		{
			printf("failed to allocate memory.. buff - func reading\n");
			//close(*(int *)connfd);
			//free(bufflen);
			//exit(0);

		}

		bzero(buff, *bufflen);

		//read data into buff
		int ret = read(*(int *)connfd, (void *)buff, *bufflen);
		if(ret == -1)
		{
			perror("read :");
			//free(bufflen);
			//free(buff);
			//exit(0);
			break;
		}
		printf("\n\t\t\t %s\n", buff);
		
		// compare and check for "bye" to exit chat
		if(!(strncmp("bye", buff , 3)))
		{
			flag = 0;
			//free(buff);
			//free(bufflen);
			//exit(0);
			break;
			//return NULL;
		}
		bzero(buff, *bufflen);
		free(buff);
	}

	free(bufflen);
	pthread_exit(NULL);
}


void *writing(void *connfd)
{
	int flag = 1;
	
	// init buffer with a set size
	char *buff = malloc(MAX);
	if(buff == NULL)
	{
		printf("failed to allocate memory.. - buff - func writing\n");
		close(*(int *)connfd);
		exit(0);

	}


	int *bufflen = malloc(sizeof(int));

	while(flag)
	{

		printf("\n");

		int current_size = MAX;

		bzero(buff, MAX);

		int i = 0;

		// get the message and store it in buffer
		while (( buff[i++] = getchar() ) != '\n')
		{
			if(i == current_size)
			{
				current_size = i + MAX;
				// reallocate memory if set mem size exceeded
				buff = realloc(buff, current_size);
				if(buff == NULL)
				{
					printf("failed to reallocate memory..\n");
					//close(*(int *)connfd);
					//free(buff);
					//exit(0);
					break;
				}
			}
		}

		buff[i] = '\0';

		//printf("buffer value = %s", buff);
		printf("i = %d\n", i);
		// store message length in bufflen
		*bufflen = i;

		//printf("bufflen = %d\n", *bufflen);
		
		// send the bufflen 
		if((write(*(int *)connfd, bufflen, sizeof(int))) == -1)
		{
			perror("write");
			//free(buff);
			//free(bufflen);
			//exit(0);
			break;
		}
		
		// write the content
		int wri1 = write(*(int *)connfd, buff, i);
		if(wri1 == -1)
		{
			perror("write : ");
			//close(*(int *)connfd);
			//free(buff);
			//free(bufflen);
			//exit(0);
			break;
		}
	
		// compare msg for "bye" to exit chat
		if(!(strncmp("bye", buff , 3)))
		{
			flag = 0;
			//free(buff);
			//free(bufflen);
			//close(*(int *)connfd);
			//exit(0);
			break;
			//return NULL;

		}
		bzero(buff, i);
	}

	free(buff);
	free(bufflen);
	pthread_exit(NULL);
}


void chat_func(int connfd)
{

	printf("\n\n  ***********Chat APP started**********\n\n");
	printf("\nsend 'bye' to exit chat app\n");
	// creating threads
	thread1 = pthread_create(&t1, NULL, (void *)reading, (void *)&connfd);
	if(thread1 != 0)
	{
		perror("thread1");
	}

	thread2 = pthread_create(&t2, NULL, (void *)writing, (void *)&connfd);
	if(thread2 != 0)
	{
		perror("thread2");
	}

	// terminating the threads
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);


	printf("Chat ended\n\n");

}

int main()
{

	int sockfd, connfd, reuse1=1;
	socklen_t len;	
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = create_socket();

	// to reuse socket with same address
	//int reuse1 = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse1, sizeof(reuse1)) < 0)
		perror("setsockopt(SO_REUSEADDR) failed");

	memset(&servaddr, '\0', sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	// long integer from host byte order to network byte order.
	servaddr.sin_port = htons(PORT1);	// short integer from host byte order to network byte order.

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0)
	{
		printf("socket bind failed...\n");
		close(sockfd);
		exit(0);
	}
	else
	{
		printf("Socket successfully binded..\n");
	}

	// server listens
	if ((listen(sockfd, 0)) != 0) 
	{
		printf("Listen failed...\n");
		close(sockfd);
		exit(0);
	}
	else
	{
		printf("Server listening..\n");
	}

	len = sizeof(cli);

	// Accept the data from client

	while((connfd = accept(sockfd, (struct sockaddr*)&cli, &len)) != -1)
	{

		if (connfd < 0) 
		{
			printf("server accept failed...\n");
			close(sockfd);
			exit(0);
		}
		else
		{
			printf("server accepted the client...\n");
		}


		printf("chat function started\n");

		chat_func(connfd);

	}

return 0;
}
