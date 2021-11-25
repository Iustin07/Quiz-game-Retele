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
#include <vector>
#define MAX 1024
#define PORT 8135
#define SA struct sockaddr 
enum error{
BIND,
CONNECT,
SOCKET,
LISTEN,
FOPEN,
ACCEPT,
FCLOSE,
READ,
Write
};
using namespace std;
pthread_mutex_t mutex;
int score_Array[3], inde = 0;
int score_i = 0, iter = 0, maxi = 0;
int clients[3], clients_i = 0;
char *candidate[3];
vector<pthread_t>threading;
vector<void*>sums(20,0);
// Function for sending winner message
void sendtoall(char * msg);

void* func(void * client_sock) ; 
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 
 int x=0;
	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		return SOCKET;
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// prepare structure
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 
 int optval=1; 
	 setsockopt (sockfd, SOL_SOCKET, (SO_REUSEADDR|SO_REUSEPORT), &optval, sizeof(optval)); 
	//bind socket
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		return BIND;
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 100)) != 0) { 
		printf("Listen failed...\n"); 
		return LISTEN;
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 
 int t=0;
 int k;
	while(1){
		// Accept clients
         k=0;
        while(k<3){//try to group clients to create rooms of 3
	    connfd = accept(sockfd, (struct sockaddr * ) NULL, NULL);
		if (connfd < 0) { 
			printf("server acccept failed...\n"); 
			return ACCEPT; 
		}
		else
			printf("server acccept the client...\n");
            pthread_t server_thread; 
		clients[clients_i] = connfd;
        //cout<<"Am adaugat clientul: "<<connfd<<" la lista de clienti\n";		
        threading.push_back(server_thread);
        //cout<<"am pus thread-ul "<<server_thread<<" in vector\n";
         pthread_create(&threading.at(clients_i),NULL,&func,&clients[clients_i]);
         clients_i++;
        ++k;
		
        }
        cout<<"out\n";
             
			for(int i=t;i<threading.size();++i)
            {
                pthread_join(threading.at(i),&sums.at(i));
                int x=(long)sums.at(i);
                cout<<"Client score is: "<<x<<endl;
                score_Array[score_i]=x;
                score_i++;
            }
		
		int maximum = -1, found = 0;
		int i = 0;
		while(i < score_i){
			if(score_Array[i] > maximum){
				maximum = score_Array[i];//maximum score
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
		strcpy(message, candidate[found]);//send to all the anme of the winner
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
          t=k+t;
          //threading.clear();
		  //sums.clear();
	}

	close(sockfd); 
}
void sendtoall(char * msg) {//function which will send the name of the winner
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
{ cout<<"Am inceput functia de tratare client\n";
	int sockfd = * ((int * ) client_sock);
	fstream FD_Server, FD,REG;
	char buff[MAX], ANS[MAX], remove_File[MAX];
	string fromFile, Name ,toFile, fileNameis;
	int n, ans;
	int x = 0, candidate_score = 0;
    // char mess[31]="please insert your username:";
    // write(sockfd, mess, sizeof(mess));
	// read the message from client and copy it in buffer 
		read(sockfd, buff, sizeof(buff)); 
		// print buffer which contains the client contents 
		printf("From client: %s\t To client : ", buff); 
		Name = buff;
		strcpy(remove_File, buff);
		strcpy(ANS, buff);
		fileNameis = Name;
		candidate[iter] = ANS;
		int curr_iter = iter;
		iter++;
		REG.open("config_file",ios::out|ios::app);//register part
        REG<<buff;
		cout << "Name of file :" << Name << endl;
		FD.open(Name, ios::out | ios::app);
		bzero(buff, MAX);
		FD_Server.open("Quiz.txt", ios::in);
		cout << "\nFileOpen successfully 1\n";
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
				//  send  buffer to client 
				write(sockfd, buff, sizeof(buff)); 
				if(strcmp(buff, "#") == 0){
					FD.close();
					fstream FD_Server1, FD1;
					char xyz[32];
					string k, cli;
					n = 0;
					int ok=0;
					FD_Server1.open("Key.txt", ios::in);
					FD1.open(fileNameis, ios::in);
					while(1){//answers compare
						cout << "Enter in ...";
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
				// send it to client 
				write(sockfd, buff, sizeof(buff)); 
				cout << "Choice send to client :" << buff << '\n';
				if(fromFile == "#"){
					cout << "in # ";
					break;
				}
				fromFile.clear();
				
				read(sockfd, buff, sizeof(buff)); 
				// print buffer which contains the client contents 
				printf("From client: %s\t To client : ", buff);
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
		REG.close();
		FD.close();
          //cout<<"Gata functia\n";
		remove(remove_File);
		pthread_exit ((void*)(candidate_score));
      
}
