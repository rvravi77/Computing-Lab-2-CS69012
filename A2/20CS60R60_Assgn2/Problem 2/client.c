//Roll no :20CS60R60
//Name : Ravi Pratap Singh
//A2
//Compilation command for client :  gcc client.c -o client 
//Running command for client :   ./client PORT
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#define WORD 256
/*****************************************************************************************
 * 
 *                                      MAIN
 * 
 * ***************************************************************************************/
int main(int argc, char **argv)
{	
    int PORT;
    if(argc<2)
        {
            printf ("Too less argvs \n");
            exit(1);
        }
    else{
            PORT= atoi(argv[1]);
    }	
	//VARIABLE DECLARATIONS
	int sock;										//socket file descriptor
	struct sockaddr_in address;						//structure for socket address details
	int client_sock;								//new file descriptor returned by accept
	char *buffer;							        //buffers for storing strings										
   
    buffer=(char *)malloc(WORD*sizeof(char));       //Allocating memory for character arrays

	sock=socket(AF_INET,SOCK_STREAM,0);				//AF_INET = for IPV4 operations			
	if(sock< 0){									//SOCK_STREAM = for stream/connection based sockets
        perror("SOCKET ERROR ");
        exit(1);
    }
    printf("Socket established\n");

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)      //Setting options for the socket
       perror("setsockopt(SO_REUSEADDR) failed");

    //setting IP address and port details for the socket

    memset((void *)&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);	//INADDR_ANY binds socket to all available interfaces
    address.sin_port = htons(PORT);

    //Connecting to the server

    if(connect(sock, (struct sockaddr *)&address, sizeof(address))< 0) {
        perror("CONNECT ERROR");
        return -1;
    }
    printf("Client Connected\n");

    while(1){  
       
        memset(buffer,'\0',WORD);
        printf("Enter Message To Server (\"EXIT\" to stop) :\n");       
        fgets(buffer,WORD,stdin);
		if ((strlen(buffer) > 0) && (buffer[strlen (buffer) - 1] == '\n'))  //removing endline of fgets()
        	buffer[strlen (buffer) - 1] = '\0';

        //Sending message to server
        write(sock,buffer,strlen(buffer));   //sending to server
        if(strcmp(buffer,"EXIT")==0)
            break;
        memset(buffer,'\0',WORD);
        read(sock,buffer,WORD);              //receiving from server
        printf("Server replied : %s\n",buffer);
    }
    //Closing socket
	close(sock);
}	