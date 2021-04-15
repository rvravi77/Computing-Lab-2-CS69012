//Roll no :20CS60R60
//Name : Ravi Pratap Singh
//A1
//Compilation command for client :  gcc client.c -o client 
//Running command for client :   ./client
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
/*****************************************************************************************
 * 
 * 
 *                                      MAIN
 * 
 * 
 * ***************************************************************************************/
int main()
{	
	//create a socket
	int network_socket;
	network_socket = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP); //0 -TCP
	
	struct hostent *server;
	server = gethostbyname("localhost");
	 if (server == NULL) {
        printf("No such host found !!\n");
        exit(0);
    }
	//specify an address for the socket
	struct sockaddr_in server_address;
	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family=AF_INET;
	bcopy((char *)server->h_addr,(char *)&server_address.sin_addr.s_addr,server->h_length);
	server_address.sin_port = htons(5003); // PORT

	//server_address.sin_addr.s_addr = INADDR_ANY; //connect to 0.0.0.0

	if ( connect(network_socket,(struct sockaddr *) & server_address ,sizeof(server_address)) < 0){
		printf("There was an error making a connection\n\n");
		exit(0);
	}
	else 
		printf("YAYY!! .. Connection established\n\n");
	char server_response[256];
	char hello[256];
	while(1){
	//Send to server
	printf("Enter Request(eg: \"EVALUATEX 12\" (0 to n) or \"WRITEX 2+2*3\" or \"stop me\" ):"); 
	bzero(hello,256);
	fgets(hello, 256, stdin);
	write(network_socket , hello , strlen(hello) ); 
    printf("Sent to server :%s\n\n",hello); 
	//receive data from server
	bzero(server_response, 256);
	read(network_socket,&server_response,sizeof(server_response ));
	printf("The server sent the data : %s \n", server_response); //print out the server's response
	if(strcmp("stopped",server_response) == 0){
		close(network_socket); //close the socket
		printf("Disconnected from the server\n");
		exit(0);
		}
	}
}	