//20CS60R60
//A1T1
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{	
	//create a socket
	int network_socket;
	network_socket = socket(AF_INET, SOCK_STREAM,0); //0 -TCP
	
	//specify an address for the socket
	struct sockaddr_in server_address;
	server_address.sin_family=AF_INET;
	server_address.sin_port = htons(9002); // PORT -9002
	server_address.sin_addr.s_addr = INADDR_ANY; //connect to 0.0.0.0

	int connection_status = connect(network_socket,(struct sockaddr *) & server_address ,sizeof(server_address));
	
	//check for error with the connection 
	if(connection_status == -1)
	{
		printf("There was an error making a connection\n\n");
		exit(0);
	}
	else 
	{
		printf("YAYY!! .. Connected to server\n\n");
	}

	//Send to server
	char *hello = "Hello from client"; 
	send(network_socket , hello , strlen(hello) , 0 ); 
    printf("Sent to server :%s\n\n",hello); 

	//receive data from server
	char server_response[256];
	
	recv(network_socket,&server_response,sizeof(server_response ),0);

	//print out the server's response
	printf("The server sent the data : %s : serv ip :%s\n", server_response,inet_ntoa(server_address.sin_addr));

	//close the socket
	close(network_socket); 
	return 0;

}	
