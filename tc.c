// TCP client code for chatting application 

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>


#define MAX 100
#define PORT 8080

//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *read_thread(int sockfd)
{
//	pthread_mutex_lock(&lock);

	char buff[MAX];	// buff to enter the message that we need to send
	memset(buff, '\0', MAX);
	printf("inside read thread\n");
sleep(1);	
	// read buffer data
	if((read(sockfd, buff, MAX)) == -1)
	{
		printf("failed to read data\n");
//		free(len);
	//	close(sockfd);
		//infi_flag = 0;
	//	exit(0);
		//printf("exiting..\n");
	}

	printf("Server : %s\n", buff);

	memset(buff, '\0', MAX);

	printf("exit read thread\n");

//	pthread_mutex_unlock(&lock);

}

void *write_thread(int sockfd)
{
//	pthread_mutex_lock(&lock);

	printf("inside write thread\n");
	char buff[MAX];	// buff to enter the message that we need to send
	memset(buff, '\0', sizeof(buff)); 

	printf("\nClient : ");


	// take input into buffer
	int i=0;

//	while((buff[i++] = getchar()) != '\n')
//		;
//	buff[i]	= '\0';		
//	buff[MAX] = "hello 1";
	scanf("%s", buff);

	// send the msg which is stored in buffer 
	if((write(sockfd, buff, sizeof(buff))) == -1)
	{
		printf("failed to write data\n");
//		free(len);
	//	close(sockfd);
		//infi_flag = 0;
	//	exit(0);
		//printf("exiting..\n");

	}

	memset(buff, '\0', MAX);

	printf("write thread exited\n");

//	pthread_mutex_unlock(&lock);
}



// function to chat between client and server

void chat_func(int sockfd)
{
	int infi_flag = 1;

	//memset(buff, '\0', sizeof(buff));	
	printf("\n\n\n************ TCP Client *************\n");
	
	printf("Chat rules : \n 1) Send an empty message when you are done sending messages.\n 2) send 'bye' to exit the chat.\n\n ");

	// threads

	pthread_t thread1, thread2;


	
while(infi_flag)
	{	

	
		int tid1 = pthread_create(&thread1, NULL, (void *)write_thread, &sockfd);

		sleep(5);

		int tid2 = pthread_create(&thread2, NULL, (void *)read_thread, &sockfd);
		
		sleep(5);	
//		pthread_join(thread1, NULL);
//		pthread_join(thread2, NULL);

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

