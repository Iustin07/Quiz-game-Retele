#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <string>
#define MAX 1024
#define PORT 8306
#define SA struct sockaddr 

using namespace std;
pthread_mutex_t mutex;
int score_Array[3], inde = 0;
int score_i = 0, iter = 0, maxi = 0;
int clients[3], clients_i = 0;
char *candidate[3];


// Function fro sending winner message. 
void sendtoall(char * msg);
void* func(void * client_sock);
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	//prepare structure
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 
 int optval=1; 
	 setsockopt (sockfd, SOL_SOCKET, (SO_REUSEADDR|SO_REUSEPORT), &optval, sizeof(optval));
	 //setsockopt (sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));  
	//bind socket
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// listen for clients
	if ((listen(sockfd, 100)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	while(1){
		pthread_t th1, th2, th3, th4;
		// Accept  
		connfd = accept(sockfd, (struct sockaddr * ) NULL, NULL);
		if (connfd < 0) { 
			printf("server acccept failed...\n"); 
			exit(0); 
		}
		else
			printf("server acccept the client...\n"); 
		clients[clients_i] = connfd;
		clients_i++;
		pthread_create( & th1, NULL, &func, & connfd);
		
		// Accept 
		connfd = accept(sockfd, (struct sockaddr * ) NULL, NULL);
		if (connfd < 0) { 
			printf("server acccept failed...\n"); 
			exit(0); 
		} 
		else
			printf("server acccept the client...\n");
		clients[clients_i] = connfd;
		clients_i++;
		pthread_create( & th2, NULL, &func, & connfd);
		
		// Accept  
		connfd = accept(sockfd, (struct sockaddr * ) NULL, NULL);
		if (connfd < 0) { 
			printf("server acccept failed...\n"); 
			exit(0); 
		} 
		else
			printf("server acccept the client...\n");
		clients[clients_i] = connfd;
		clients_i++;
		pthread_create( & th3, NULL, &func, & connfd);
			
		void *sum1 = NULL;
		pthread_join (th1, &sum1);
		int x1 = (long) sum1;
		cout << "Client1 score is =" << x1 << endl;
		score_Array[score_i] = x1;
		score_i++;
		
		void *sum2 = NULL;
		pthread_join (th2, &sum2);
		int x2 = (long) sum2;
		cout << "Client2 score is =" << x2 << endl;
		score_Array[score_i] = x2;
		score_i++;
		
		void *sum3 = NULL;
		pthread_join (th3, &sum3);
		int x3 = (long) sum3;
		cout << "Client3 score is =" << x3 << endl;
		score_Array[score_i] = x3;
		score_i++;
			
		int maximum = -1, found = 0;
		int i = 0;
		while(i < score_i){
			if(score_Array[i] > maximum){
				maximum = score_Array[i];
				found = i;
			}
			i += 1;
		}
		cout << "Score_Array is =";
		for(int i = 0; i < score_i; i++){
			cout << score_Array[i] << " ";
		}
		cout << "Maximum score is =" << maximum << endl;
		char message[500];
		strcpy(message, candidate[found]);
		sendtoall(message);
		for(int i = 0; i < 3; i++) {
			memset(candidate[i], 0 , strlen(candidate[i]));
		}
		for(int i = 0; i < 3; i++) {
			score_Array[i] = 0;
		}
		for(int i = 0; i < 3; i++) {
			clients[i] = 0;
		}
		score_i = 0;
		iter = 0;
		clients_i = 0;
	}

	
	close(sockfd); 
}
/***********************/
void sendtoall(char * msg) {
	int i;
	pthread_mutex_lock( & mutex);
	for (i = 0; i < clients_i; i++) {
		if (send(clients[i], msg, strlen(msg), 0) < 0) {
			printf("sending failure \n");
			continue;
		}
	}
	pthread_mutex_unlock( & mutex);
}
void* func(void * client_sock) 
{ 
	int sockfd = * ((int * ) client_sock);
	fstream FD_Server, FD,REG;
	char buff[MAX], ANS[MAX], remove_File[MAX];
	string fromFile, Name ,toFile, fileNameis;
	int n, ans;
	int x = 0, candidate_score = 0;
	// read the message from client and copy it in buffer 
		read(sockfd, buff, sizeof(buff)); 
		// print buffer-> client message 
		printf("From client: %s\t To client : ", buff); 
		Name = buff;
		strcpy(remove_File, buff);
		strcpy(ANS, buff);
		fileNameis = Name;
		candidate[iter] = ANS;
		int curr_iter = iter;
		iter++;
		REG.open("config_file",ios::out|ios::app);
        REG<<buff;
		cout << "Name of file :" << Name << endl;
		FD.open(Name, ios::out | ios::app);
		bzero(buff, MAX);
		FD_Server.open("Quiz.txt", ios::in);
		cout << "\nFile Open success!\n";
		if(FD_Server){
			while(1){
				getline(FD_Server, fromFile);
				cout << endl;
				cout << "Question from file :" << fromFile << endl;
				
				n = 0; 
				// copy server message in the buffer 
				while ((fromFile[n]) != '\0') {
					buff[n] = fromFile[n]; 
					n++;
				}
				// send  buffer to client 
				write(sockfd, buff, sizeof(buff)); 
				if(strcmp(buff, "#") == 0){
					//cout << "Enter in # system :" << fileNameis << endl;
					FD.close();
					fstream FD_Server1, FD1;
					char xyz[32];
					string k, cli;
					n = 0;
					FD_Server1.open("Key.txt", ios::in);
					FD1.open(fileNameis, ios::in);
					while(1){
						//cout << "Enter in ...";
						getline(FD_Server1, k);
						getline(FD1, cli);
						
						cout << "Key is :" << k << endl;
						cout << "client ans is :" << cli << endl;
						
						if(k == "#"){
							break;
						}
						else if(k == cli){
							x += 1;
						}
						k.clear();
						cli.clear();
					}
					cout << "Client score is :" << x << endl;
					cout << "Client name is :" << candidate[curr_iter] << endl;
					candidate_score = x;
					
					FD_Server1.close();
					FD1.close();
					
					string X = to_string(x);
					n = 0;
					while ((X[n]) != '\0') {
						buff[n] = X[n]; 
						n++;
					}
					cout << "Client score to send is: " << buff << endl;
					write(sockfd, buff, sizeof(buff));
					break;
				}
				cout << "Question send to client :" << buff << '\n';
				read(sockfd, buff, sizeof(buff)); 
				fromFile.clear();
				
				getline(FD_Server, fromFile);
				cout << endl;
				cout << "Choice from file :" << fromFile << '\n';
				n = 0; 
				// copy server message in the buffer 
				while ((fromFile[n]) != '\0') {
					buff[n] = fromFile[n]; 
					n++;
				}
				// and send that buffer to client 
				write(sockfd, buff, sizeof(buff)); 
				cout << "Choices send to client :" << buff << '\n';
				if(fromFile == "#"){
					cout << "Enter in #...";
					break;
				}
				fromFile.clear();
				
				read(sockfd, buff, sizeof(buff)); 
				// print buffer which contains the client contents 
				printf("Choice from client: %s\t Next q/a send to client : ", buff);
				toFile = buff;
				FD << toFile;
				//ANS[ans] = toFile;
				//ans++;
				 
				toFile.clear();
				fromFile.clear();
				bzero(buff, MAX);
			}
		}
		else{
			cout << "File not found" << endl;
		}
		FD_Server.close();
		FD.close();
		REG.close();
		remove(remove_File);
		pthread_exit ((void*)(candidate_score));
}