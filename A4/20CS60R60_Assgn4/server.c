//Roll no :20CS60R60
//Name : Ravi Pratap Singh
//A4
//Compilation command for server :  gcc server.c -o server
//Running command for server :      ./server PORT

#include<stdio.h>           //for performing input and output
#include<stdlib.h>          //for general functions
#include<string.h>          //for strlen ,strcmp, strcpy
#include<ctype.h>           //for testing and mapping characters.
#include<sys/socket.h>      //for sockaddr structure
#include<sys/types.h>       //for defination of data types
#include<netinet/in.h>      //for sockaddr_in structure
#include<fcntl.h>           //for for O_RDONLY ,O_WRONLY
#include<unistd.h>          //for open and close
#include<errno.h>           //for printing error
#include<arpa/inet.h>       //for in_addr structure
#include<netinet/tcp.h>     //for TCP_NODELAY
#include<sys/stat.h>        //for getting file size using stat
#include<dirent.h>          //for type representing a directory stream.
//#include<sys/sendfile.h>    //for sendfile()
#include <sys/uio.h>
#include<time.h>            //for time
#include<sys/time.h>        //for gettimeoftheday

//macro definations
#define WORD 512            //Maximum size of command, filename , buffer
#define NO_OF_CLIENT 20     //Maximum numbers of client that can be connected

/**********************************************************************************
 *                           Utility functions                                    *
 * ********************************************************************************/

//function to get integer code corresponding to the command
int get_command(char *command){
	//populated value valriable to indicate back to main which input was entered
    if(strncmp(command, "RETR",4) == 0){return 1;}
    else if(strncmp(command, "STOR",4) == 0){return 2;}
    else if(strncmp(command, "LIST",4) == 0){return 3;}
    else if(strncmp(command, "DELE",4) == 0){return 6;}
    else if(strncmp(command, "CODEJUD",7) == 0){return 7;}
    else {return 8;}
}

//Function for DELE command
int do_dele(char *str,int client_sock,int client_num){
    struct stat obj;                                //Object to store file attributes
    int size;                                       //Stores size of file
    int filefd;                                     //file descriptor for file to be deleted
    char *filename;                                 //stores filename to be deleted
    char *command;                                  //for giving command to kernal
    filename=(char *)malloc(WORD*sizeof(char));     //dynamic allocation of memory for variables
    command=(char *)malloc(WORD*sizeof(char));
    //getting file attributes
    strncpy(filename,str+5,strlen(str)-5);          
    stat(filename, &obj);

	filefd = open(filename, O_RDONLY);              //open file
	size = obj.st_size;                             //get file size
	if(filefd == -1)                                //check if file exists
	    size = 0;
    if(size){
        sprintf(command,"rm -f ./%s",filename);          
        system(command);                            //system call to remove file
        printf("File %s successfully deleted\n\n",filename);
	    if( send(client_sock, &size, sizeof(int), 0) < 0){   //send success flag
            perror("SEND: ");
            exit(0);
        }
    }
    else{
        printf("No such file here\n\n");
        if( send(client_sock, &size, sizeof(int), 0) < 0){   //send failure flag
            perror("SEND: ");
            exit(0);
        }
    }
    return size;
}
/***********************************************************************************
 *                                      MAIN                                        *
 * ********************************************************************************/
int main(int argc, char **argv)
{	
    int PORT;                                       //stores port number input from command line 
    if(argc<2){
        printf ("Too less argvs \n");
        exit(1);
    }
    else{
            PORT= atoi(argv[1]);
    }

    char *str;                                      //stores the string received from client
    char *command;                                  //stores the actual command from string
    char *filename;                                 //stores the filename from the string
    char *language;
    int client_socket[NO_OF_CLIENT];                //array to store socket of client 
    int client_number[NO_OF_CLIENT];                //array to store index number of client
    int max_fd;							            //maximum fd for select() command
	int client_fd;						            //new client fd to accept connection
    int client_count=0;					            //count of number of clients 
    fd_set readfds;						            //set of file descriptors to read

    //dynamically allocating memory for buffers
    str=(char *)malloc(WORD*sizeof(char));
	command=(char *)malloc(WORD*sizeof(char));
    filename=(char *)malloc(WORD*sizeof(char));
    language=(char *)malloc(WORD*sizeof(char));

    //create server socket
	int server_socket;
	struct sockaddr_in server_address;              //define the server address
    
    //Setting details of IP address or port for the socket
    memset((void *) &server_address, 0,sizeof(server_address));
	server_address.sin_family =AF_INET;                                     // IPV4
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);                     //address (binds socket to all available interfaces)
    server_address.sin_port = htons(PORT);                                  //Port no 

    //socket
    server_socket = socket(AF_INET,SOCK_STREAM,0);
    if(server_socket < 0){
        perror("SOCKET: ");
        exit(1);
    }

    printf("Server Started... \n\n");
    //setting up socket options
    setsockopt(server_socket, IPPROTO_TCP,TCP_NODELAY,   &(int){1}, sizeof(int));
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,  &(int){1}, sizeof(int));

	//bind the socket to the speified IP and port
	if (bind(server_socket ,(struct sockaddr*) &server_address,sizeof(server_address)) < 0 ){
        perror("BIND: ");
        close(server_socket);
        exit(0);
    }

    printf("Socket bound to Port %d...\n\n",PORT);
	if(listen(server_socket, 20) < 0){                                      //connection waiting at particular time
        perror("LISTEN: ");
        exit(1);
    }

    printf("Listening....\n\n");
    //setting all client sockets and client numbers to 0 initially
    for(int i=0;i<NO_OF_CLIENT;i++){
    	client_socket[i]=0;
    	client_number[i]=0;
    }


    struct sockaddr_in cli_addr;                                            //address of client
    socklen_t clilen;                                                       //storing length for client address, i.e. 32 bit integer
    clilen = sizeof(cli_addr); 

    while(1){
        FD_ZERO(&readfds);				                                    //clearing the fd set
		FD_SET(server_socket,&readfds);	                                    //adding server socket to fd set
		max_fd=server_socket;
		for(int i=0;i<NO_OF_CLIENT;i++){
			if(client_socket[i]>0){		                                    //adding all valid client sockets to fd set
				FD_SET(client_socket[i],&readfds);  
				if(client_socket[i]>max_fd){
					max_fd=client_socket[i];
				}
			}
		}
                            printf("Waiting ...\n");
		if(select(max_fd+1,&readfds,NULL,NULL,NULL) < 0){                   //select the ready client file descriptor
			perror("SELECT: ");
			exit(1);
		}

        if(FD_ISSET(server_socket,&readfds)){                               //returns file descriptor set pointed to by readfds
            //incomming connection is in server socket
            client_count++;
			client_fd=accept(server_socket,(struct sockaddr *) &cli_addr,&clilen);
			printf("Connected to client  %d\n",client_count);
			fflush(NULL);
			for(int i=0;i<NO_OF_CLIENT;i++){                                //populate client socket untill an empty client found
				if(client_socket[i]==0){
					client_socket[i]=client_fd;
					client_number[i]=client_count;
					break;
				}					
			}
		}

        else{
			for(int i=0;i<NO_OF_CLIENT;i++){
				if(FD_ISSET(client_socket[i],&readfds)){                    //reading from client file descriptor
                    //setting WORD bytes of sting to NULL
		            memset(str,'\0',WORD);
                    memset(command,'\0',WORD);
                    memset(filename,'\0',WORD);
                    memset(language,'\0',WORD);

            		if(recv(client_socket[i],str,WORD,0) < 0){              //receive string from client
                        perror("READ :");
                        exit(1);
                    }

                    //strncpy(command,str,4);                                 //get actual command from string
                    sscanf(str,"%s",command);
		            printf("%s Command received from Client %d \n",command,client_number[i]);

		            if(strcmp(str,"QUIT")==0){                              //If a QUIT command received then exit
                        printf("Client %d disconnected\n",client_number[i]);
                        close(client_socket[i]);                            //control connection
						client_socket[i]=0;
			            break;
		            }	
                    
                    int cmd = get_command(command);                         //stores integer value corresponding to command
                    int size=0;                                             //stores the file size for RETR/STOR
                    int filefd;                                             //file descriptor for opening and closing file
                    int c;                                                  //flag for success or failure message
                    int idx_file=1;                                         //append value for duplicate files
                    char *f;                                                //stores the file temporarly for writing
                    struct stat obj;                                        //object to store file attributes
                    DIR *d;                                                 //directory stream object
                    struct dirent *dir;                                     //storing format of directories


                    switch(cmd){

                        case 1:                                             //for RETR
                            strncpy(filename,str+5,strlen(str)-5);          //getting file name from string
                            filefd = open(filename, O_RDONLY);
	                        stat(filename, &obj);                           //initializing file attributes
	                        size = obj.st_size;                             //getting file size
                            
	                        if(filefd == -1)
	                            size = 0;
	                        if( send(client_socket[i], &size, sizeof(int), 0) < 0){  //sending file size to client
                                perror("SEND: ");
                                break;
                            }
                            //if file exists then sending to client
	                        if(size){
                                printf("Sending file %s to client %d\n",filename,client_number[i]);
	                            if( sendfile(client_socket[i], filefd,0,NULL ,NULL, size) < 0){
                                    perror("SEND: ");
                                    break;
                                }
                            }
                            else
                                printf("No such file here\n\n");
                            close(filefd);
                        break;


                        case 2:                                             //for STOR
                            strncpy(filename,str+5,strlen(str)-5);          //getting file name from string
	                        if(recv(client_socket[i], &size, sizeof(int), 0) < 0){  //receive filesize
                                perror("RECV: ");
                                break;
                            }
                            //creating file (if already exit append '1' to filenmae)
	                        while(1){
	                            filefd = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
	                            if(filefd == -1){
		                            sprintf(filename + strlen(filename), "%d", idx_file);
	                            }
	                            else break;
	                        }
	                        f = malloc(size+WORD);                               //memory allocation for file size
	                        if( recv(client_socket[i], f, size, 0) < 0){    //receive file from client
                                perror("SEND: ");
                                break;
                            }
                            c = write(filefd, f, size);                     //write into file
	                        close(filefd);              
                            if( send(client_socket[i], &c, sizeof(int), 0) < 0){ //sends number of bytes written
                                perror("SEND: ");
                                break;
                            }
                            if(c)
	                            printf("File %s stored successfully by client %d\n\n",filename,client_number[i]);
	                        else
	                            printf("File %s failed to be stored to remote host\n\n",filename);
                        break;
	                        

                        case 3:                                             //for LIST
                            d = opendir(".");                               //opening current directory
                            if (d){
                                while ((dir = readdir(d)) != NULL){         
                                    sprintf(filename,"%s %s\n",filename, dir->d_name);
                                }
                                closedir(d);
                            }
                            //sending list of files of current directory of server
                            if( send(client_socket[i], filename , strlen(filename), 0) < 0){
                                perror("SEND: ");
                                break;
                            }
                            printf("Sending list of files to Client %d\n\n",client_number[i]);
                        break;


                        case 6:                                             //for DELE
                            do_dele(str,client_socket[i],client_number[i]);
                        break;


                        case 7:                                             //for CODEJUD

                            //getting command , filename , language from string
                            memset(command,'\0',WORD);
                            sscanf(str,"%s %s %s",command,filename,language);    
	                        if(recv(client_socket[i], &size, sizeof(int), 0) < 0){  //receive filesize
                                perror("RECV: ");
                                break;
                            }
                            //creating file 
	                        filefd = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);

                            truncate(filename, size);                      //truncate file size to designated one

	                        f = malloc(size+WORD);                              //memory allocation for file size
	                        if( recv(client_socket[i], f, size,0) < 0){    //receive file from client
                                perror("RECV: ");
                                break;
                            }
                            c = write(filefd, f, size);                     //write into file
                            fdatasync(filefd);
	                        close(filefd);                                  //close file FD
                            if( send(client_socket[i], &c, sizeof(int), 0) < 0){ //sends number of bytes written
                                perror("SEND: ");
                                break;
                            }
                            if(c>size  || c<0){                                  //if some error in storing file 
	                            printf("File %s failed to be stored to remote host\n\n",filename);
                                memset(command,'\0',WORD);                       //remove file if incorrectly stored
                                sprintf(command,"rm -f ./%s",filename);
                                system(command);
                                break;
                            }

                                char *token=strtok(filename,".");               //tokenize filename and extension

                                memset(command,'\0',WORD);
                                if(!strcmp(language,"c"))                       //creation of command for system call
                                    sprintf(command,"gcc %s.c -o %s_%d",token,token,client_number[i]);
                                else if(!strcmp(language,"c++"))
                                    sprintf(command,"g++ %s.cpp -o %s_%d",token,token,client_number[i]);

                                int compile = system(command);                  //system call for compilation
                                
                                //sending compile success or fail to client
                                if( send(client_socket[i], &compile,sizeof(int),0) < 0){
                                    perror("SEND: ");
                                    break;
                                }

                                if(compile == 0)
                                    printf("Compile success\n");
                                else{                                            //if compile not success then delete the stores file
                                    printf("Compile not success\n");
                                    memset(command,'\0',WORD);
                                    if(!strcmp(language,"c"))
                                        sprintf(command,"rm -f ./%s.c",token); 
                                    else if(!strcmp(language,"c++"))
                                        sprintf(command,"rm -f ./%s.cpp",token); 
                                    system(command);
                                    break;
                                }
                            
                                memset(command,'\0',WORD);
                                sprintf(command,"input_%s.txt",token);          //input file for redirecting input
                                FILE *fp_ip=fopen(command,"r");                 //File pointer for input file
                                char ch[256];                                   //character array to store each line input
                                int run;                                        //stores exit status of run command
                                int run_flag=0;                                 //stores runtime error
                                int tle_flag=0;                                      //stores time limit exceed flag
                                if(fp_ip!=NULL){                                //checking does code requires input file or not
                                    while( EOF !=  fscanf(fp_ip, "%[^\n]\n", ch))
                                    {
                                        struct timeval  tv1, tv2;               //keep track of execution time
                                        FILE *fp2=fopen("temp_file_for_input.txt","w+");
                                        fprintf(fp2,"%s\n",ch);                 //storing text in new file
                                        fclose(fp2);
                                        memset(command,'\0',WORD);

                                        //running obejct file with input redirection from new file
                                        sprintf(command,"timeout 5.0s ./%s_%d 0<temp_file_for_input.txt 1>>output_%s_%d.txt",token,client_number[i],token,client_number[i]);


                                        gettimeofday(&tv1, NULL);               //get start time
                                        run = system(command);                  //system call to run
                                        gettimeofday(&tv2, NULL);               //get end time


                                        system("rm -f ./temp_file_for_input.txt");   //remove temporary file
                                        memset(ch,'\0',256);                

                                        //calculating time taken
                                        float tc= (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);

                                        //either runtime exceed or error in running cause runtime error
                                        if(run!=0)
                                            run_flag=1;
                                        if(tc>1)
                                            tle_flag=2;
                                    }
                                }
                                else{                                           //if code does'nt require input filr
                                    struct timeval  tv1, tv2;
                                    memset(command,'\0',WORD);
                                    sprintf(command,"timeout 5.0s ./%s_%d 1>>output_%s_%d.txt",token,client_number[i],token,client_number[i]);


                                    gettimeofday(&tv1, NULL);                   //get start time
                                    run = system(command);                      //system call to run
                                    gettimeofday(&tv2, NULL);                   //get end time

                                    //calculating time taken
                                    float tc= (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
                                    //either runtime exceed or error in running cause runtime error
                                    if(run!=0)
                                        run_flag=1;
                                    if(tc>1)
                                        tle_flag=2;
                                }
                                
                                //sending run status to client
                                if(run_flag){
                                    if( send(client_socket[i], &run_flag,sizeof(int),0) < 0){
                                        perror("SEND: ");
                                        break;
                                    }
                                    printf("Run time error in some testcase\n %d %d",run_flag,tle_flag);
                                }else if(tle_flag){
                                    if( send(client_socket[i], &tle_flag,sizeof(int),0) < 0){
                                        perror("SEND: ");
                                        break;
                                    }
                                    printf("TLE error in some testcase\n");
                                }else{
                                    if( send(client_socket[i], &run_flag,sizeof(int),0) < 0){
                                        perror("SEND: ");
                                        break;
                                    }
                                    printf("Run success\n");
                                }

                                if(run_flag==1 || tle_flag==1){
                                    memset(command,'\0',WORD);

                                    //if run did'nt succeed then remove code file, object file and output file
                                    if(!strcmp(language,"c"))
                                        sprintf(command,"rm -f ./%s.c ./%s_%d ",token,token,client_number[i]); 
                                    else if(!strcmp(language,"c++"))
                                        sprintf(command,"rm -f ./%s.cpp ./%s_%d ",token,token,client_number[i]); 
                                    system(command);
                                    break;
                                }

                                int match_flag=0;                               //flag to store match status of all test case
                                memset(command,'\0',WORD);
                                sprintf(command,"cat testcase_%s.txt",token);
                                if( system(command) !=0 ){
                                    match_flag=2;
                                }
                                else{
                                    //comparing output file with testcase file using system call 'awk'
                                    memset(command,'\0',WORD);
                                    sprintf(command,"awk '!NF{next} FNR==NR{a[FNR]=$1;next};{print $1==a[FNR]  ? 1 : 0}' output_%s_%d.txt testcase_%s.txt 1>temp_file_for_output.txt",token,client_number[i],token);
                                    system(command);                                //gives boolean output for each test case

                                    FILE *fPtr1;                                    //file pointer for temporary output file
                                    fPtr1 = fopen("temp_file_for_output.txt","r");
                                    memset(ch,'\0',256);
                                    while(EOF != fscanf(fPtr1,"%[^\n]\n",ch)){
                                        if(atoi(ch)==0){                            //if '0' found implies one or more testcases failed
                                            match_flag=1;
                                            break;
                                        }
                                        memset(ch,'\0',256);
                                    }
                                    fclose(fPtr1);
                                }

                                //sending matching status to client
                                if( send(client_socket[i], &match_flag,sizeof(int),0) < 0){
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

                                memset(command,'\0',WORD);
                                //removing actual code file and other files created during the process
                                if(!strcmp(language,"c"))
                                    sprintf(command,"rm -f ./%s.c ./%s_%d ./temp_file_for_output.txt ",token,token,client_number[i]); 
                                else if(!strcmp(language,"c++"))
                                    sprintf(command,"rm -f ./%s.cpp ./%s_%d ./temp_file_for_output.txt ",token,token,client_number[i]); 
                                system(command);
                            
                        break;


                        case 8:
                            printf("Enter Valid Request \n\n");
                        break;
                    }
                    fflush(NULL);
                }
            }
        }
	}
	close(server_socket);  
}