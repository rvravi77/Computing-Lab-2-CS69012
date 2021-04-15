//20CS60R60
//A1T1
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

int main(){
	char server_message[256]= "You have reached the server";

	//create server socket
	int server_socket;
	server_socket = socket(AF_INET,SOCK_STREAM,0);
	
	//define the server address
	struct sockaddr_in server_address;
	server_address.sin_family =AF_INET;
	server_address.sin_port = htons(9002);
	server_address.sin_addr.s_addr = INADDR_ANY;

	 if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
       perror("setsockopt(SO_REUSEADDR) failed");

	//bind the socket to the speified IP and port
	if (bind(server_socket ,(struct sockaddr*) &server_address,sizeof(server_address)) < 0 ){
        perror("bind");
        close(server_socket);
        exit(0);
    }

	listen(server_socket, 2); // connection waiting at particular time

	int client_socket;
	printf("Waiting for client to Connect ..\n");
	client_socket = accept(server_socket,NULL,NULL);
	printf("A client is connected to the server\n");
	sleep(1);
	printf("Waiting for client to send a message ..\n");
	sleep(1);
	char buffer[1024] = {0}; 
	read( client_socket , buffer, 1024); 
    printf("Client sent : %s\n\n",buffer ); 
	//send a message
	printf("Message sent to client : %s \n\n",server_message);
	send(client_socket, server_message, sizeof(server_message),0);
	printf("Closing .. \n");
	sleep(2);
	//close the socket
	close(server_socket);
	
	return 0;
}
