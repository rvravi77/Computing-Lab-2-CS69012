//Roll no :20CS60R60
//Name : Ravi Pratap Singh
//A1
//Compilation command for server :  gcc server.c -o server -lm
//Running command for server :   ./server
//input file should be in same folder saved as input.txt
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<ctype.h>
#include<math.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include<assert.h>
/********************************************************************
 * 
 *                        STACK DECLARATION
 * 
 * ******************************************************************/
// A structure to represent a operand stack
struct intStack {
    int top;
    double array[100];
};
// A structure to represent a operator stack
struct charStack {
    int top;
    char array[100];
};
 
// Function to add operend item to the stack.  
void pushnum(struct intStack* stack, double item){
    stack->array[++stack->top] = item;
}
// Function to add an operator to the stack. 
void pushop(struct charStack* stack, char item){
    stack->array[++stack->top] = item;
}
// Function to remove an item from the stack.  
int popnum(struct intStack* stack){
    if (stack->top == -1)
        return INT_MIN;
    return stack->array[stack->top--];
}
// Function to remove an item from the stack
int popop(struct charStack* stack){
    if (stack->top == -1)
        return INT_MIN;
    return stack->array[stack->top--];
}
// Function to return the top from operator stack without removing it
int chartop(struct charStack* stack){
    if (stack->top==-1)
        return INT_MIN;
    return stack->array[stack->top];
}
// Function to return the top from operand stack without removing it
double inttop(struct intStack* stack){
    if (stack->top==-1)
        return INT_MIN;
    return stack->array[stack->top];
}
/********************************************************************
 * 
 *                    EXPRESSION EVALUATION
 * 
 * ******************************************************************/
int white_sp(char c){
    return c == ' '; // to ignore any white spaces
}
int is_op(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%'; // to check for operator
}
int is_unary(char c) {
    return c == '+' || c=='-'; //to check for unary operator
}
//to check proirity among operator
int priority (char op) {
    if (op < 0) 
        return 4;  // unary operator - highest priority
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/' || op=='%')
        return 2;
    if (op == '^')
        return 3; 
    return -1;
}
//funtion to perform the operation
void process_op(struct intStack* st, char op) {
    if (op < 0) {
        // If a unary operator
        double l = inttop(st); popnum(st);
        switch (-op) {
            case '+': pushnum(st,l); break;
            case '-': pushnum(st,-l); break;
        }
    } else {
        //pop two elemnts from stack and perform operation
        double r = inttop(st); popnum(st);
        double l = inttop(st); popnum(st);
        switch (op) {
            case '+': pushnum(st,l + r); break;   // push to stack after performing operation
            case '-': pushnum(st,l - r); break;
            case '*': pushnum(st,l * r); break;
            case '/': pushnum(st,l / r); break;
            case '^': pushnum(st,pow(l,r)); break;
            case '%': pushnum(st,(int)l%(int)r); break;
        }
    }
}
// Function to push/pop into operator and operand stack according to priority
//also perform the nesscessary oprations
double evaluate(struct intStack* s1,struct charStack* s2, char *s) {
    
    if(s[0]==')')
        return INT_MIN;

    int unary_flag = 1; // flag to check for the unary operator
    for (int i = 0; i < strlen(s); i++) {
        if (white_sp(s[i]))
            continue;  // if  a white space then ignore
        if (s[i] == '(') {                 // if opening braces then push into operator stack
            pushop(s2,'(');
            unary_flag = 1;
        } else if (s[i] == ')') {
            while (chartop(s2) != '(') {      // if closing braces then perform operation untill opening braces found
                process_op(s1, chartop(s2));
                popop(s2);
            }
            popop(s2);
            unary_flag = 0;
        } else if (is_op(s[i])) {               // if an operator(+ ,-  ,/ ,* ,% ,^)
            char cur_op = s[i];
            if (unary_flag && is_unary(cur_op))  // if unary then negate
                cur_op = -cur_op;
            while (!(s2->top==-1) && (
                    (cur_op >= 0 && priority(chartop(s2)) >= priority(cur_op)) ||
                    (cur_op < 0 && priority(chartop(s2)) > priority(cur_op))        //loop untill a lesser priority operator found
                )) {                                
                process_op(s1,chartop(s2));         //perform operation 
                popop(s2);
            }
            pushop(s2,cur_op);
            unary_flag = 1;
        } else {                            // if an operand( 1 , 2, 2.33 , 666.70  , 0.99)
            double number = 0,n2=0;
            int cnt = 0;
            while (i < strlen(s) && isalnum(s[i]))  {  // convert string to double
                    number = number * 10 + s[i++] - '0';
                    if(s[i]=='.')
                        break;
                }
            if(s[i]=='.') {   //convert the floating point number
                i++;
                while (i < strlen(s) && isdigit(s[i])){
                    n2 = n2 * 10 + s[i++] - '0';
                    cnt++;
                }
                cnt=pow(10,cnt);
                n2=n2/cnt;
                number=n2+number;
            }
            --i;
            pushnum(s1,number);  // push the operand into stack
            unary_flag = 0;
        }
    }
    // after scaaning complete infix expression , pop and perform operation on remaning elements
    while (!(s2->top==-1)) {
        process_op(s1, chartop(s2));
        popop(s2);
    }
    //the result lies in top of operand stack
    return inttop(s1);
}
void chopN(char *str, size_t n)
{
    assert(n != 0 && str != 0);
    size_t len = strlen(str);
    if (n > len)
        return;  // Or: n = len;
    memmove(str, str+n, len - n + 1);
}

int removep(char *s){
if(s[0]!='(')
   return -1;
int i=0;
while(s[i]== '(')
   i++;
i=strlen(s)-1;
int count=0;
while (s[i]==')')
    {count++; i--;}
return count;
}
/*****************************************************************************************
 *
 *                                      MAIN
 * 
 * ***************************************************************************************/
int main(){
	char server_message[256];
	//create server socket
	int server_socket,valread;
	//define the server address
	struct sockaddr_in server_address;
    bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family =AF_INET;  // IPV4
	server_address.sin_addr.s_addr = INADDR_ANY; //address 
    server_address.sin_port = htons(5003); // Port no 
    //socket
    server_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    printf("Server Started .. \n\n");
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
       perror("setsockopt(SO_REUSEADDR) failed");

	//bind the socket to the speified IP and port
	if (bind(server_socket ,(struct sockaddr*) &server_address,sizeof(server_address)) < 0 ){
        perror("bind");
        close(server_socket);
        exit(0);
    }

	listen(server_socket, 1); // connection waiting at particular time
    struct sockaddr_in cli_addr;
    socklen_t clilen; //storing length for client address, i.e. 32 bit integer
    clilen = sizeof(cli_addr);
	int client_socket;
    //accept
	client_socket = accept(server_socket,(struct sockaddr *) &cli_addr,&clilen);
    printf("A client is connected to the server\n");
    char FILENAME[25];
    FILE* file = fopen("inpu.txt","r+");
    if(file == NULL){
        printf("File not found ,Enter file Name :");
        scanf("%s",FILENAME);
        file= fopen(FILENAME, "r+");
    }
    do
    {
    printf("Waiting for client to send command...\n");
    fseek(file, 0, SEEK_SET);
	char buffer[256]; 
    bzero(buffer,256);
    // Receive data from client
	while(read( client_socket , buffer, 256) <=0)
        client_socket = accept(server_socket,(struct sockaddr *) &cli_addr,&clilen); 
    printf("Client sent  : %s \n\n",buffer );
    
    if( strcmp(buffer,"stop") == 32){
        strcpy(server_message,"stopped");
        write(client_socket, server_message, sizeof(server_message));
        continue;
        }
	//tokenizing the string received from client
	int scan=0,lineNumber=0;
	char rqst[256];
	while(buffer[scan]!=' ' && buffer[scan]!='\0'){
		rqst[scan]=buffer[scan];
		scan++;
	    }
	rqst[scan++]='\0';

    //////////////////////////////////////////////////////////////////////
    //if EVALUATEX
	if(strcasecmp("EVALUATEX",rqst)==0) { 
        //stack pointer declaration
	    struct intStack s1;
        struct charStack s2;
        s1.top=-1;
        s2.top=-1;
		 while (scan < strlen(buffer) && isdigit(buffer[scan])){
            lineNumber = lineNumber * 10 + buffer[scan++] - '0';        
		    }
        // evaluating expression
	    int count = 0;
	    char line[256]; // or other suitable maximum line size 
        double result;
        while (fgets(line, sizeof(line), file) != NULL) {// read a line 
            if (count == lineNumber){
                int x= strlen(line);
                line[x-1]='\0';
                result= evaluate(&s1,&s2,line);
                if(result <= (-21474836)  )
                    strcpy(server_message,"Invalid Expression in File");
                else
                    gcvt(result, 256,  server_message);
                break;
            }
            else
                count++;      
        }
        if (feof(file)){
            printf("\n Reached to the end of file. ENTER VALID LINE NUMBER !!!\n \n");
            strcpy(server_message,"ENTER VALID LINE NUMBER !!!");
        }
        fflush(stdin);
	}
    ////////////////////////////////////////////////////////////////////////////////
	else{                                       //if WRITEX
        if(strcasecmp("WRITEX",rqst)==0){
            //stack pointer declaration
	        struct intStack s1;
            struct charStack s2;
            s1.top=-1;
            s2.top=-1;
            char line[256], querybuf[256], temp[256]; // or other suitable maximum line size 
            int p=0,flag=0,inval=0;
            while(scan <strlen(buffer)){  // remove white space
                if(buffer[scan]==' ')
                  { scan++; continue; }
                querybuf[p++]=buffer[scan++];
                }
            querybuf[p]='\0';
            scan=0;
            // check for invalid input character
            while(querybuf[scan]!='\n'){
                if(querybuf[scan]=='(' ||  querybuf[scan]==')' || isalnum(querybuf[scan]) || is_op(querybuf[scan]) )
                    {scan++; }
                else
                    {inval=1; break;}
            }
            querybuf[scan]='\0';
            if(inval==1)
               { strcpy(server_message,"Invalid characters in expression\n");
                 write(client_socket, server_message, sizeof(server_message)); continue; }
            // check for extra parenthesis
            int x1= removep(querybuf);
            if(x1>0){
                chopN(querybuf,x1);
                querybuf[strlen(querybuf)-x1]='\0';}
            
            fseek(file, 0, SEEK_SET);
            strcat(querybuf, "\n"); 
            // printf("qb %s",querybuf); debug
            while (fgets(line, sizeof(line), file) != NULL) {
                    int i = 0, j = 0;           //removing white spaces
	                while (line[i]){             
		                if (line[i] != ' ')
                        line[j++] = line[i];
		                i++;
                    }
	                line[j] = '\0';
                    if(strcmp(querybuf,line)==0){
                            flag=1;  break;
                    }
            }
            double result= evaluate(&s1,&s2,querybuf);
            if(flag==0 && (result> (-21474836))){
                fseek(file, 0, SEEK_END);
                fprintf(file, "%s", querybuf);
                strcpy(server_message,"Success!!");
                }
            else
                strcpy(server_message,"Expression already exists / Invalid expression\n"); 
	    }
        else
            strcpy(server_message,"Enter valid request \n");  
    }
    printf("Message sent to client : %s \n\n",server_message);
	write(client_socket, server_message, sizeof(server_message));  //Sending to client
    fflush(NULL);
    }while(1);
	//close the socket
    printf("closing... \n");
    fclose(file);   //close the file
    sleep(4);
	close(server_socket);
    close(client_socket);
    printf("Server Closed\n");
    return 0;
}