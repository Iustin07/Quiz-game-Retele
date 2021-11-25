#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#define MAX 1024
#define PORT 8135
#define SA struct sockaddr 
using namespace std; 
enum error{
BIND,
CONNECT,
SOCKET,
FOPEN,
FCLOSE,
READ,
Write
};

void func(int sockfd) 
{  //cout<<"call of the function \n";
	char buff[MAX]; 
	int n; 
	// read(sockfd,buff,sizeof(buff));
    // printf("Am citit: %s", buff);
	bzero(buff, sizeof(buff)); 
    strcpy(buff,"");
	printf("Please enter your username for register: "); 
	n = 0; 
	while ((buff[n++] = getchar()) != '\n'); 
	write(sockfd, buff, sizeof(buff)); 
	for (;;) { 
		
		//bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("\n\nFrom Server : %s", buff); 
		if ((strncmp(buff, "#", 4)) == 0) { 
			printf("Client exit...\n"); 
			break; 
		} 
		
		bzero(buff, sizeof(buff)); 
		strcpy(buff, ".");
		write(sockfd, buff, sizeof(buff)); 
		
		//bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("\n\nFrom Server : %s", buff); 
		if ((strncmp(buff, "#", 4)) == 0) { 
			printf("Client Exit...\n"); 
			break; 
		} 
		
		bzero(buff, sizeof(buff)); 
		printf("\nEnter Your Choice : "); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n'); 
		write(sockfd, buff, sizeof(buff)); 
	}
	///cout<<"next score will be print!\n";
	cout << endl;
	read(sockfd, buff, sizeof(buff)); 
	cout << "Client score is :" << buff << endl;
	
	cout << "Waiting for announcement of WINNER...\n";
	read(sockfd, buff, sizeof(buff)); 
	cout << "\n\n\t\tWINNER is :" << buff << endl;
} 

int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// create socket+verification of creating it
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		return SOCKET; 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

  /*structure prepare for connecting with server*/
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");/*adress to connect*/ 
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		return CONNECT;
	} 
	else
		printf("connected to the server..\n"); 

	 /*function ->receive and send messages to server*/
	func(sockfd); 

	close(sockfd); 
} 

