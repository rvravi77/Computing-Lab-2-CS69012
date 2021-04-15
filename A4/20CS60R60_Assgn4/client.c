//Roll no :20CS60R60
//Name : Ravi Pratap Singh
//A4
//Compilation command for client :  gcc client.c -o client 
//Running command for client :   ./client 127.0.0.1 PORT

#include<stdlib.h>          //for general functions
#include<stdio.h>           //for performing input and output
#include<string.h>          //for strlen ,strcmp, strcpy
#include<sys/types.h>       //for defination of data types
#include<sys/socket.h>      //for sockaddr structure
#include<netinet/in.h>      //for sockaddr_in structure
#include<netdb.h>           //for hostent structure
#include <fcntl.h>          //for O_RDONLY
#include <unistd.h>         //for open and close
#include<arpa/inet.h>       //for in_addr structure
#include<sys/stat.h>        //for getting file size using stat
//#include<sys/sendfile.h>    //for sendfile()
#include <sys/uio.h>
#include <netinet/tcp.h>    //for TCP_NODELAY
#define WORD 512            //Maximum size of command, filename , buffer

//function to get integer code corresponding to the command
int get_command(char *command){
	//populated value valriable to indicate back to main which input was entered
    if(strncmp(command, "RETR" ,4) == 0) { return 1;}
    else if(strncmp(command, "STOR",4) == 0){ return 2;}
    else if(strncmp(command, "LIST",4) == 0){ return 3;}
    else if(strncmp(command, "DELE",4) == 0){ return 6;}
    else if(strncmp(command, "CODEJUD",7) == 0){ return 7;}
    else { return 8;}
}

/*****************************************************************************************
 *                                      MAIN                                             *
 * ***************************************************************************************/

int main(int argc, char **argv){    	
    int PORT;                                       //stores port number input from command line                                                   
    if(argc<3){
        printf ("Too less argvs \n");
        exit(1);
    }
    else{
        PORT= atoi(argv[2]);                        //convert port number to integer
    }	
	//VARIABLE DECLARATIONS
	int sock;										//socket file descriptor
	struct sockaddr_in address;						//structure for socket address details
	char *buffer;							        //buffers for storing strings	
    char *command;                                  //for storing the input command	
    char *filename;                                 //for storing filename	
    char *language;

    //Dynamically Allocating memory for character arrays
    buffer=(char *)malloc(WORD*sizeof(char));       
    command=(char *)malloc(WORD*sizeof(char));      
    filename=(char *)malloc(WORD*sizeof(char));    
    language=(char *)malloc(WORD*sizeof(char));    

	sock=socket(AF_INET,SOCK_STREAM,0);				//AF_INET = for IPV4 operations			
	if(sock< 0){									//SOCK_STREAM = for stream/connection based sockets
        perror("SOCKET: ");
        exit(1);
    }
    printf("Socket established\n");

    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &(int){1}, sizeof(int));      //Setting options for the socket
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR , &(int){1}, sizeof(int));         

    //setting IP address and port details for the socket
    memset((void *)&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(argv[1]);
    address.sin_port = htons(PORT);

    //Connecting to the server
    if(connect(sock, (struct sockaddr *)&address, sizeof(address))< 0) {
        perror("CONNECT: ");
        return -1;
    }
    printf("Client Connected\n");

    while(1){  
        //Setting all bytes in buffer to null
        memset(buffer,'\0',WORD);
        memset(filename,'\0',WORD);
        memset(command,'\0',WORD);
        memset(language,'\0',WORD);

        printf("Enter Message To Server (RETR;STOR;LIST;QUIT;DELE;CODEJUD):\n");     //Prompt to get input from user       
        fgets(buffer,WORD,stdin);
		if ((strlen(buffer) > 0) && (buffer[strlen (buffer) - 1] == '\n'))   //removing endline of fgets()
        	buffer[strlen (buffer) - 1] = '\0';

        //Sending QUIT message to server
        if(strcmp(buffer,"QUIT")==0 && buffer[4]=='\0'){
            if( send(sock,buffer,strlen(buffer),0) < 0){
                perror("SEND: ");
                break;
            }
            break;
        }

        //If not a QUIT message
        //strncpy(command,buffer,4);
        int cmd=get_command(buffer);               //Storing integer code corresponding to command  
        int size=0;                                 //Stores size of file retreived or stores
        int filehandle=0;                           //file descriptor for the file sent or received
        int i=1;                                    //to append at end of file name in case of same file
        int status=0;                               //to get the success or fail message from server
        struct stat obj;                            //obejct declaration for file attributes
        int compile_flag;
        int run_flag;
        int match_flag;

        switch(cmd){


            case 1:                                                          //for RETR 
                if(buffer[4]!=' '){
                    printf("ENTER VALID REQUEST \n");
                    break;
                }                         
                if(send(sock,buffer,strlen(buffer),0) < 0){                  //send the command to server
                    perror("SEND: ");
                    break;
                }
                if( recv(sock,&size,sizeof(int),0) < 0 ){                    //receive the size of file from server
                    perror("RECV: ");
                    break;
                }
                if(!size){                          
	                printf("No such file on the remote directory\n\n");
	                break;
        	    }
                strncpy(filename,buffer+5,strlen(buffer)-5);                 //get filename from command            
                char *f;                                                     //to allocate memory for file
                f = malloc(size+WORD);                                            //memory allcocated of size got from server
	            if( recv(sock, f, size, 0) < 0){                             //Receive file data into buffer
                    perror("RECV: ");
                    break;
                }
	            while(1){
	                filehandle = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
	                if(filehandle == -1){
		                sprintf(filename + strlen(filename), "%d", i);       //appends 1 at end if same file exists
            		}
	                else break;
	            }
	            write(filehandle, f, size);                                  //Write into file from buffer 'f'
	            close(filehandle);                                           //close the file
                printf("Received file %s from server\n\n",filename); 
            break;


            case 2:                                                          //for STOR     
                if(buffer[4]!=' '){
                    printf("ENTER VALID REQUEST \n");
                    break;
                }  
                strncpy(filename,buffer+5,strlen(buffer)-5);                 //get filename from command
        	    filehandle = open(filename, O_RDONLY);                       //Checking does the file exists
                if(filehandle == -1){
                    printf("No such file on the local directory\n\n");
                    break;
                }
                if( send(sock,buffer,strlen(buffer),0) < 0){                  //send command to server if file exist
                    perror("SEND: ");
                    break;
                }
	            stat(filename, &obj);                                        //getting file attributes into object
	            size = obj.st_size;                                          //st_size gets the file size
	            if( send(sock, &size, sizeof(int), 0) < 0){                   //send file size to server
                    perror("SEND: ");
                    break;
                }
	            if( sendfile(sock, filehandle, 0,NULL,NULL ,size) < 0){              //send the file data to server
                    perror("SEND: ");
                    break;
                }
	            if( recv(sock, &status, sizeof(int), 0) < 0 ){                 //receive the success or failure flag
                    perror("SEND: ");
                    break;
                }
	            if(status)
	                printf("File stored successfully\n");
	            else
	                printf("File failed to be stored to remote host\n");
                close(filehandle);
            break;


            case 3:                                                          //for LIST
                if(buffer[4]!='\0'){
                    printf("ENTER VALID REQUEST \n");
                    break;
                }  
                if( send(sock,buffer,WORD,0) < 0){                           //send command to server
                    perror("SEND: ");
                    break;
                }
                if( recv(sock,filename,WORD,0) < 0){                         //receiving from server
                    perror("RECV: ");
                    break;
                }
                printf("list of files from server==>\n");
                printf("%s\n\n",filename);                                   //print list of all files
            break;


            case 6:                                                          //for DELE
                 if(buffer[4]!=' '){
                    printf("ENTER VALID REQUEST \n");
                    break;
                }  
                printf("Do you really wanna delete (y/n) ? :");              //Prompt to comfirm delete operation
                char ch[2];                                                       
                gets(ch);                                                    //get y/n
                if(strcasecmp(ch,"y") == 0){
                    if( send(sock,buffer,strlen(buffer),0) < 0){             //send delete command
                        perror("SEND: ");
                        break;
                    }
                    strncpy(filename,buffer+5,strlen(buffer)-5);             //getting filename from command 
                    if( recv(sock, &status, sizeof(int), 0) < 0){             //getting success or failure status from server
                        perror("SEND: ");
                        break;
                    }
                    if(status)      
                        printf("\nFile %s successfully deleted\n\n",filename);
                    else    
                        printf("\nNo such file on server\n\n");
                }
            break;

            case 7:
                sscanf(buffer,"%s %s %s",command,filename,language);
                if(buffer[7]!=' '){
                    printf("ENTER VALID REQUEST \n");
                    break;
                }
                filehandle = open(filename, O_RDONLY);                       //Checking does the file exists
                if(filehandle == -1){
                    printf("No such file on the local directory\n\n");
                    break;
                }
                if( strlen(language)<= 0 || (strcmp(language,"c") &&  strcmp(language,"c++"))){
                    printf("LANGUAGE NOT SUPPORTED !!\n");
                    break;
                }
                if( send(sock,buffer,strlen(buffer),0) < 0){                  //send command to server if file exist
                    perror("SEND: ");
                    break;
                }
	            stat(filename, &obj);                                         //getting file attributes into object
	            size = obj.st_size;                                           //st_size gets the file size
	            if( send(sock, &size, sizeof(int), 0) < 0){                   //send file size to server
                    perror("SEND: ");
                    break;
                }
	            if( sendfile(sock, filehandle, 0, NULL,NULL, size) < 0){              //send the file data to server
                    perror("SEND: ");
                    break;
                };
                close(filehandle);
	            if( recv(sock, &status, sizeof(int), 0) < 0 ){                 //receive the success or failure flag
                    perror("SEND: ");
                    break;
                }
	            if(status>size || status <0)
	                {
                        printf("File failed to be stored to remote host\n");
                        break;
                    }
                if( recv(sock, &compile_flag, sizeof(int), 0) < 0 ){                 //receive the success or failure flag
                    perror("SEND: ");
                    break;
                }
                if(compile_flag == 0 )
                    printf("Compile success\n");
                else{
                    printf("Compile not success\n");
                    break;
                }
                
                if( recv(sock, &run_flag, sizeof(int), 0) < 0 ){                 //receive the success or failure flag
                    perror("SEND: ");
                    break;
                }
                if(run_flag == 0)
                    printf("Run success\n");
                else if(run_flag ==1){
                    printf("Run time error in some testcase\n");
                    break;
                }else if(run_flag ==2){
                    printf("TLE error in some testcase\n");
                    break;
                }
                
                if( recv(sock, &match_flag, sizeof(int), 0) < 0 ){                 //receive the success or failure flag
                    perror("SEND: ");
                    break;
                }
                //printing match status on client side
                if (match_flag == 0)
                    printf("All test Case passed\n");
                else if(match_flag == 1 )
                    printf("Not all test Cases passed\n");
                else if(match_flag == 2)
                    printf("Testcase file not present\n");
            break;

            break;


            case 8:                                                          //if invalid input from user
                printf("Enter Valid Request \n\n");
            break;
            
        }
        fflush(NULL);
    }
	close(sock);                                                             //Closing socket
}	