//Roll no :20CS60R60
//Name : Ravi Pratap Singh
//A2
//Compilation command for server :  gcc server.c -o server -lm
//Running command for server :   ./server PORT
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<ctype.h>
#include<math.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<fcntl.h> // for open
#include<unistd.h> // for close
#include<assert.h>
#include<time.h>
#include<errno.h>
#include<arpa/inet.h>
//macro definations
#define WORD 256
#define OUTPUT_FILE "server_records.txt"
#define NO_OF_CLIENT 20
char * in_to_post(char  *);
char *eval_postfix(char *);
/********************************************************************
 * 
 *            Stack and Utility Function DECLARATION
 * 
 * ******************************************************************/
// A structure to represent a operator stack
struct charStack {
    int top;
    char array[100];
};
 
// Function to add an operator to the stack. 
void pushop(struct charStack* stack, char item){
    stack->array[++stack->top] = item;
}

// Function to remove an item from the stack
int popop(struct charStack* stack){
    if (stack->top == -1)
        return INT_MIN;
    return stack->array[stack->top--];
}
// Function to return the top from operator stack without removing it
char chartop(struct charStack* stack){
    //if (stack->top==-1)
      //  return INT_MIN;
    return stack->array[stack->top];
}
int is_op(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%'; // to check for operator
}
//to check proirity among operator
int priority (char op) {

    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/' || op=='%')
        return 2;
    if (op == '^')
        return 4; 
    if (op == '#')
        return 3;              //highest priority
    return -1;
}
//to identify unbalanced parenthesis
int identifyParenthesis(char *a){
    int count1=0,count2=0;
    for (int i = 0; i < strlen(a); i++) {
        if (a[i] == '(') 
            count1++;
        else if (a[i] == ')') 
            count2++;
    }
    return count1==count2;
}
//to chop first N characters from string
void chopN(char *str, size_t n){
    assert(n != 0 && str != 0);
    size_t len = strlen(str);
    if (n > len)
        return;
    memmove(str, str+n, len - n + 1);
}
/********************************************************************
 * 
 *                          MAIN
 * 
 * ******************************************************************/
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

    char *str,*buffer,*buffer2,*temp;   //for string inputs from client and buffer for storing intermediate expressions
    //dynamically allocating memory for buffers
    str=(char *)malloc(WORD*sizeof(char));
	buffer=(char *)malloc(WORD*sizeof(char));
    buffer2=(char *)malloc(WORD*sizeof(char));
	temp=(char *)malloc(512*sizeof(char));

    clock_t start,end;				    //start,end times for client request
	double duration;				    //elapsed times for client request
    int client_socket[NO_OF_CLIENT];    //socket of client 
    int client_number[NO_OF_CLIENT];    //index number of client
    int x;                              //fork variable
    int max_fd;							//maximum fd for select()
	int client_fd;						//new client fd
    int client_count=0;					//number of clients
    fd_set readfds;						//set of file descriptors to read

    //create server socket
	int server_socket,valread;
	struct sockaddr_in server_address;  //define the server address
    
    //Setting details of IP address or port for the socket
    memset((void *) &server_address, 0,sizeof(server_address));
	server_address.sin_family =AF_INET;                     // IPV4
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);     //address (binds socket to all available interfaces)
    server_address.sin_port = htons(PORT);                  // Port no 

    //socket
    server_socket = socket(AF_INET,SOCK_STREAM,0);
    if(server_socket<0){
        perror("Socket Error ");
        exit(1);
    }

    printf("Server Started .. \n\n");

    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
       perror("setsockopt(SO_REUSEADDR) failed");

	//bind the socket to the speified IP and port
	if (bind(server_socket ,(struct sockaddr*) &server_address,sizeof(server_address)) < 0 ){
        perror("Bind Error");
        close(server_socket);
        exit(0);
    }

    printf("Socket bound to Port ...\n\n");

	if(listen(server_socket, 20) < 0){   //connection waiting at particular time
        perror("Listen Error\n");
        exit(1);
    }

    printf("Listening....\n\n");

    //setting all client sockets to 0 initially
    for(int i=0;i<NO_OF_CLIENT;i++){
    	client_socket[i]=0;
    	client_number[i]=0;
    }
    FILE *fp=fopen(OUTPUT_FILE,"w");

    struct sockaddr_in cli_addr;        //address of client
    socklen_t clilen;                   //storing length for client address, i.e. 32 bit integer
    clilen = sizeof(cli_addr); 

    while(1){
        FD_ZERO(&readfds);				//clearing the fd set
		FD_SET(server_socket,&readfds);	//adding server socket to fd set
		max_fd=server_socket;
		for(int i=0;i<NO_OF_CLIENT;i++){
			if(client_socket[i]>0){		//adding all valid client sockets to fd set
				FD_SET(client_socket[i],&readfds);
				if(client_socket[i]>max_fd){
					max_fd=client_socket[i];
				}
			}
		}

		if(select(max_fd+1,&readfds,NULL,NULL,NULL) < 0){
			perror("Select error :");
			exit(1);
		}

        if(FD_ISSET(server_socket,&readfds)){               // returns file descriptor set pointed to by readfds
            //incomming connection is in server socket
			client_fd=accept(server_socket,(struct sockaddr *) &cli_addr,&clilen);
			client_count++;
			printf("Connected with client socket number %d\n",client_count);
			fflush(NULL);
			for(int i=0;i<NO_OF_CLIENT;i++){
				if(client_socket[i]==0){
					client_socket[i]=client_fd;
					client_number[i]=client_count;
					break;
				}					
			}
		}

        else{
			for(int i=0;i<NO_OF_CLIENT;i++){
				if(FD_ISSET(client_socket[i],&readfds)){    //reading from client FD
		            memset(str,'\0',WORD);
                    memset(temp,'\0',WORD);
            		if(read(client_socket[i],str,WORD)<0){
                        perror("READ :");
                        exit(1);
                    }
		            if(strcmp(str,"EXIT")==0)
		            {
			            printf("Client socket number %d exits\n",client_number[i]);
                        close(client_socket[i]);
						client_socket[i]=0;
			            break;
		            }
		            //START OF CLIENT REQUEST
		            start=clock();		
		            printf("Client socket number %d sent message : %s\n",client_number[i],str);
                    int count = 0; 
                    for (int i = 0; i < strlen(str); i++) {
                        if (str[i] != ' ') 
                            str[count++] = str[i]; 
                        }
                    str[count] = '\0'; 
                    buffer=in_to_post(str);         //function call to convert into postfix
                    if(buffer[0]==' '){             //removing first blank space from postfis expression
                        chopN(buffer,1);
                        buffer[strlen(buffer)]='\0';
                    }
                    buffer2=eval_postfix(buffer);               //function call to evaluate postfix expression
                    if(strcasecmp(buffer2,"ERROR")==0)
                        buffer="ERROR";
                    sprintf(temp,"%s , %s\n",buffer,buffer2);   //return meaage to client
                    printf("Sending reply:  %s",temp);          
                    write(client_socket[i],temp,strlen(temp));  //writing on client FD
		            end=clock();
		            //END OF CLIENT REQUEST
		            duration=(double)(end - start)/(double)CLOCKS_PER_SEC;
		            fprintf(fp,"<%s;%d> \t\t\t\t%s\t\t\t\t%s\t\t\t\t%s\t\t\t\t%lf sec\n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port),str,buffer,buffer2,duration);
		            fflush(NULL);
                }
            }
        }
	}
	close(server_socket);  
}

/********************************************************************
 * 
 *                    INFIX TO POSTFIX
 * 
 * ******************************************************************/

char * in_to_post(char  *s) {

    struct charStack  op;               //stack declaration
    char *ret;                          //return string
    op.top=-1;                          //initialize top of stack to -1
    ret=(char *)calloc(512,sizeof(char));//memory allocation for return vairable
    if(!identifyParenthesis(s)){        //Checks for imbalanced parenthesis
			ret="ERROR";
			return ret;
		} 

    for (int i = 0; i < strlen(s); i++){//loop scanning entire string

        if(isalpha(s[i])!=0){           // If an invalid character present
			ret="ERROR";
			return ret;
		}
        if(i==0 && s[0]=='-'){          //unary minus at front of string
            pushop(&op,'#');
            double number=0.0;
            sprintf(ret,"%s %lf",ret,number);
            continue;
        }

        if (s[i] == '(') {
            if(s[i+1]==')')
                return "ERROR";
            pushop(&op,'(');
            if(s[i+1]=='-'){             // unary minus in begining of '('
                pushop(&op,'#');         // push special character to denote unsary minus(highest priority)
                double number=0.0;          
                sprintf(ret,"%s %lf",ret,number);             //add zero (0) to string
                i++;
                continue;
            }
        } 
        else if (s[i] == ')'){               
                while (chartop(&op) != '(' && !(op.top==-1)){ //untill opening braces found pop all elements
                    if( chartop(&op) == '#')                  // if a unary minus
                        sprintf(ret,"%s %c",ret,'-');
                    else
                        sprintf(ret,"%s %c",ret,chartop(&op));
                    popop(&op);
                }
            popop(&op);                                       //pop ')'
            } 
            
            else if (is_op(s[i])) {                           //if operator
                    char cur_op = s[i];
                    //pop untill lesser priority operand found
                    while (!(op.top ==-1) && priority(chartop(&op)) >= priority(cur_op)) { 
                        if( chartop(&op) == '#')              // if a unary minus
                            sprintf(ret,"%s %c",ret,'-');
                        else
                            sprintf(ret,"%s %c",ret,chartop(&op));
                        popop(&op);
                    }
                    if( i>0 && !is_op(s[i-1]) )               //push the current character if no two consequetive operand
                        pushop(&op,cur_op);
                    else{
                        ret="ERROR";
        			    return ret;
                    }
                } 
                else if(isdigit(s[i])){                       //if a digit

                        if(i>1 &&  !is_op(s[i-1])  && s[i-1]!='('){
                                ret="ERROR";
                            return ret;
                        }
                        double number = 0,n2=0;
                        int cnt=0;
                        while (i < strlen(s) && isdigit(s[i])){
                            number = number * 10 + s[i++] - '0';
                            if(s[i]=='.')
                                break;
                        }
                        if(s[i]=='.'){
                          //convert the floating point number
                            i++;
                            while (i < strlen(s) && isdigit(s[i]))
                            {   
                                n2 = n2 * 10 + s[i++] - '0';
                                cnt++;
                            }
                            cnt=pow(10,cnt);
                            n2=n2/cnt;
                            number=n2+number;
                        }
                        --i;
                        sprintf(ret,"%s %lf",ret,number);
                        //invalid placement of operator
                        if(i<(strlen(s)-1) &&  !is_op(s[i+1])  && s[i+1]!=')') {
                                ret="ERROR";
                                return ret;
                            }
                    }
                    else{
			            ret="ERROR";
        			    return ret;
		            }
    }
    if(strlen(ret)==0)   
        return "ERROR";
    //pop remaning characters from stack
    while (!(op.top==-1)) {
        if( chartop(&op) == '#')    // if a unary minus
            sprintf(ret,"%s %c",ret,'-');
        else
            sprintf(ret,"%s %c",ret,chartop(&op));
        popop(&op);
    }
    return ret;
}
/********************************************************************
 * 
 *                    EXPRESSION EVALUATION
 * 
 * ******************************************************************/

char *eval_postfix(char *str)
{
	char *ret;						//return value
	char c;							//variable to store and process each character
	double arr[256];        		//array(stack) to store numbers in the postfix
	int top=-1;						//variable for  array (stack)
	double token=0;					//for number in the postfix expression
	double na,nb,nc;				//variable for calculations		

	ret=(char *)malloc(WORD*sizeof(int));   //dynamic memory allocation of return string
	for(int i=0;i<strlen(str);i++){
        if(isalpha(c)!=0){
			ret="ERROR";
			return ret;
		}
		c=str[i];
		//if current char is digit
		if(isdigit(c)!=0){
			double n2=0;
			token=0;
            int cnt=0;
            while (i < strlen(str) && isdigit(str[i])){
                token = token * 10 + str[i++] - '0';
                if(str[i]=='.')
                    break;
            }
        	if(str[i]=='.'){
               //convert the floating point number
                i++;
                while (i < strlen(str) && isdigit(str[i])){   
                    n2 = n2 * 10 + str[i++] - '0';
                    cnt++;
                }
                if(n2!=0){
                    cnt=pow(10,cnt);
                    n2=n2/cnt;
                }
                token=n2+token;
            }
			--i;
		}
        //if current char is operand
		else if(is_op(c)) 
		{
			if(((i>0)&&(str[i-1]!=' '))||((i<strlen(str)-1)&&(str[i+1]!=' '))){
				//if either previous or next character is not a blank,then error
				ret="ERROR";
				return ret;
			}
			if(top<=0){
				//if two former operands are not there,then error
				ret="ERROR";
				return ret;
			}
			else{
				//valid operator
				nb=arr[top--];
				na=arr[top--];
                switch(c){
                    case '+':
                        nc=na+nb; break;
                    case '-':
                        nc=na-nb; break;
                    case '*':
                        nc=na*nb; break;
                    case '/':
                        nc=na/nb; break;
                    case '^':
                        nc=pow(na,nb); break;
                    case '%':
                        nc=fmod(na,nb); break;

                }
                // push result in top of stack
				arr[++top]=nc;
                i++;
			}
		}
		else if(c==' '){    //when a space found put number on stack
			arr[++top]=token;
		}
	}
	arr[++top]=token;
	sprintf(ret,"%lf",arr[0]);
	return ret;
}