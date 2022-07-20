// TCP client code for chatting application 

#define _GNU_SOURCE
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>



#define MAX 100
#define PORT1 8080



int thread1, thread2;
pthread_t t1, t2;

void *reading(void *clifd)
{
	int flag = 1;

	// declare bufflen to store length of msg
	unsigned int *bufflen = malloc(sizeof(int));
	if(bufflen == NULL)
	{
		printf("failed to allocate memory to bufflen in reading\n");
		close(*(int *)clifd);
		exit(0);

	}
	bzero(bufflen, sizeof(int));

	while(flag) 
	{

		bzero(bufflen, sizeof(int));

		// read length of buffer
		if((read(*(int *)clifd, bufflen, sizeof(int))) == -1)
		{
			perror("read");
//			free(bufflen);
//			close(*(int *)clifd);
			//exit(0);
			break;
		}	
		printf("recieved len : %d\n", *bufflen);
		
		char *buff = malloc(*bufflen);
		if(buff == NULL)
		{
			printf("failed to allocate memory in buff in reading\n");
			//close(*(int *)clifd);
			//free(bufflen);
			//exit(0);

		}


		bzero(buff, *bufflen);

		//read data into buff
		int ret = read(*(int *)clifd,(void *)buff, *bufflen);
		if(ret == -1)
		{
			perror("read");
//			free(bufflen);
//			free(buff);
//			close(*(int *)clifd);
			//exit(0);
			break;
		}
		printf("\n\t\t\t%s\n", buff);

		// compare and check for "bye" to exit chat
		if(!(strncmp("bye", buff , 3)))
		{
			flag = 0;
//			close(*(int *)clifd);
//			free(bufflen);
//			free(buff);
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


void *writing(void *clifd)
{
	int flag = 1;

	// init buffer with a set size
	char *buff = malloc(MAX);
	if(buff == NULL)
	{
		printf("failed to allocate memory to buff in writting\n");
		close(*(int *)clifd);
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
					printf("failed to reallocate memory\n");
//					close(*(int *)clifd);
//					free(buff);
					//exit(0);
					break;
				}

			}
		}

		buff[i] = '\0';
	
		printf("i = %d\n", i);
		// store message length in bufflen
		*bufflen = i;

		//printf("bufflen = %d\n", *bufflen-1);

		// send the bufflen
		if((write(*(int *)clifd, bufflen, sizeof(int))) == -1)
		{
			perror("write");
//			free(buff);
//			free(bufflen);
//			close(*(int *)clifd);
			//exit(0);
			break;
		}

		// write the content
		int wri1 = write(*(int *)clifd, buff, i);
		if(wri1 == -1)
		{
			perror("write : ");
//			close(*(int *)clifd);
//			free(buff);
//			free(bufflen);			
			//exit(0);
			break;
		}
		
		// compare msg for "bye" to exit chat
		if(!(strncmp("bye", buff , 3)))
		{
			flag = 0;
			//close(*(int *)clifd);
			//free(buff);
			//free(bufflen);
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


int create_socket()
{
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
	{
		printf("Socket successfully created..\n");
		return sockfd;
	}

}

void chat_func(int clifd)
{
	
	printf("\n\n ********** Chat APP started ************ \n\n");
	printf("\n send 'bye' to exit chat app\n");

	thread1 = pthread_create(&t1, NULL, (void *)writing, (void *)&clifd);
	if(thread1 != 0)
	{
		perror("thread 1");
	}

	thread2 = pthread_create(&t2, NULL, (void *)reading, (void *)&clifd);
	if(thread2 != 0)
	{
		perror("thread 2");
	}

	// terminating the threads
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("\nChat ended\n\n");
}


// main function 
int main()
{
	int clifd, reuse1=1;

	struct sockaddr_in cliaddr;

	// client
	// socket create and verification
	clifd = create_socket();

	// to reuse socket with the same address
	if(setsockopt(clifd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse1, sizeof(reuse1)) < 0)
		perror("setsockopt(SO_REUSEADDR) failed");

	bzero(&cliaddr, sizeof(cliaddr));

	// assign IP, PORT
	cliaddr.sin_family = AF_INET;
	// enter IP of server to connect
	cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // currently on loopback
	cliaddr.sin_port = htons(PORT1);

	// connect the client socket to server socket
	if (connect(clifd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) == -1) 
	{
		printf("connection with the server failed...\n");
		close(clifd);
		exit(0);
	}
	else
	{
		printf("connected to the server..\n");
	}
	
	printf("chat function started\n");	
	chat_func(clifd);
}

