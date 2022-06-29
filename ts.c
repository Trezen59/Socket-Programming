// TCP server code for chatting application 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include<pthread.h>

#define MAX 100
#define PORT 8080

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// func to chat between client and server

void *read_thread(int connfd)
{
//	pthread_mutex_lock(&lock);

	// read the buffer and print the data                   
	printf("inside read thread\n");

	char buff[MAX];
	memset(buff, '\0', MAX);
	sleep(1);

	if((read(connfd, buff, MAX)) == -1)
	{
		printf("failed to read from client \n");
		//free(len);
//		close(connfd);
		//infi_flag = 0;
//		exit(0);
		//printf("exiting...\n");
	}

	printf("Client : %s\n", buff);
	//read the buffer and print end
	memset(buff, '\0', MAX);
	//edit

	printf("read thread exited\n");

//	pthread_mutex_unlock(&lock);
}

void *write_thread(int connfd)
{

//	pthread_mutex_lock(&lock);

	printf("inside write thread\n");

	printf("\nServer : ");
	char buff[MAX];
	memset(buff, '\0', sizeof(buff));

	// take input and store in buffer
	int i=0, flag =1;
//	while((buff[i++] = getchar()) != '\n')
//
//		;
//	buff[i] = '\0';
//	buff[MAX] = "hello";
	scanf("%s", buff);

	if((write(connfd, buff, sizeof(buff))) == -1)

	{
		printf("failed to write \n");
		//		free(len);
//		close(connfd);
		//infi_flag = 0;
//		exit(0);
		//printf("exiting...\n");
	}

	memset(buff, '\0', MAX);
	printf("write thread exited\n");

//	pthread_mutex_unlock(&lock);

}

void chat_func(int connfd)
{	

	char buff[MAX];
	int infi_flag = 1;

	printf("\n************ TCP Server *************\n");

	printf("Chat rules : \n 1) Send an empty message when you are done sending messages.\n 2) send 'bye' to exit the chat.\n\n ");

	// threads

			
	pthread_t thread1, thread2;

	while(infi_flag) 
	{
		int tid1 = pthread_create(&thread1, NULL, (void *)read_thread, &connfd);
		sleep(5);
		int tid2 = pthread_create(&thread2, NULL, (void *)write_thread, &connfd);
		sleep(5);
		//	sleep(3);
	//		pthread_join(thread1, NULL);
//		pthread_join(thread2, NULL);
	}
}

int main()
{
	int sockfd, connfd;
	socklen_t len;	
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd == -1) {
		printf("socket creation failed...\n");
		close(sockfd);
		exit(0);
	}
	else
		printf("Socket successfully created..\n");

	memset(&servaddr, '\0', sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	// long integer from host byte order to network byte order.
	servaddr.sin_port = htons(PORT);	// short integer from host byte order to network byte order.

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		close(sockfd);
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// server listens
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		close(sockfd);
		exit(0);
	}
	else
		printf("Server listening..\n");

	len = sizeof(cli);

	// Accept the data from client
	connfd = accept(sockfd, (struct sockaddr*)&cli, &len);

	if (connfd < 0) {

		printf("server accept failed...\n");
		close(sockfd);
		exit(0);
	}
	else
		printf("server accepted the client...\n");
	

	// Function to chat between client and server
	chat_func(connfd);

	// After chatting close the socket
	close(sockfd);
}


