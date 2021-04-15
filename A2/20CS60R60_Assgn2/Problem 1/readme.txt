// Computing lab II ||  CS69012  ||   Assignment #2
//Submitted by - Ravi Pratap Singh (20CS60R60 )

(on linux kernal)
Compilation command for server :  gcc server.c -o server -lm
Running command for server :   ./server PORT

Compilation command for client :  gcc client.c -o client 
Running command for client :   ./client PORT

**NOTE : 1.The server has to run first and then the client.
               2. Enter EXIT to stop the client.
               3. Response from Server is in form "postfix expression , result"
               4. "ERROR" message denotes invalid infix expression and thus invalid result
               5. data in "server_records.txt" is in the form - <client ip;client port> <infix exprn> <postfix exprn> <result> <time elapsed>
    
