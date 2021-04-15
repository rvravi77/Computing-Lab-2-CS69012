// Computing lab II ||  CS69012  ||   Assignment #4
//Submitted by - Ravi Pratap Singh (20CS60R60 )

(on linux kernal)
Compilation command for server :  gcc server.c -o server -lm
Running command for server :   ./server PORT

Compilation command for client :  gcc client.c -o client 
Running command for client :   ./client 127.0.0.1 PORT
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
For CODEJUD assumed:
1. In input_filename.txt  and testcase_filename.txt file ,each testcase lies in separate line.
2. The output file for each client and each code  is not deleted by code and saves as output_filename_CNo.txt
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- 
FTP Commands to be given as:
***CODEJUD hello.c c (or CODEJUD hello.cpp c++) {Note: space b/w CODEJUD , filename and language}
***RETR file1.txt  {Note: space b/w RETR and filename}
***STOR file2.txt  {Note: space b/w STOR and filename}
***LIST
***QUIT
***DELE file3.txt  {Note: space b/w DELE and filename}


Possible error case considered :
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CODEJUD - 1.Overwritten already present code (c or c++) file because it could hinder with input and testcase format files.
                   2. If some error in storing the file then loop breaks.
                   ########Compilation Phase:###########
                   3.Object file names as filename_clientNo so no client would overwite others object file.
                   4.If error in compilation then loop breaks , deleting code( c or c++) file.
                   ########Execution Phase :############
                   5.Created temporary file for storing each testcase input line and passing for input redirection.
                   6.Passed timeout value for running to avoid falling in infinite loops.
                   7.Kept flag for timeout/runtime error .
                   8.Output redirection done in append mode.
                   9.Output filename named as output_filename_CNo.txt to avois hinderence with other client output file for same code filename.
                   10.Handled case for code requiring input file and code not requiring it.
                   11.If either timeout or runtime error ocours then loop breaks , deleting the code file, object file.
                    #######Matching Phase : ############
                   12.Due to formatting issue of \r\n and for simplicity ,Matched output and testcase file using linux command "awk" and
                        rediredted output to an temporary file in which every line contains boolean value for matched(1) and not matched(0) testcase. 
                   13.Checked for any zero and if there exists then all testcases not passed .
                   14.Removed code file, object file and all temporary files so that it did'nt hinder with other client's files.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
RETR-  1. When already same named file exists then saved by appending one("1") at the end much like the file downloading program does. 
            2. Error when a file does'nt exist .(prompt to enter new command)
            3. Error while transfering file.        (sendfile return -1)
            4. Error while saving in client side. (write returns -1)
            5. Open file in O_RDONLY mode at server side
            6. Open with O_CREAT | O_EXCL | O_WRONLY mode at client side to create a file
STOR-  1. When already same named file exists then saved by appending one("1") at the end much like the file downloading program does. -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            2. Error when a file does'nt exist .(prompt to enter new command)
            3. Error while transfering file.        (sendfile return -1)
            4. Error while saving in server side. (write returns -1)
            5. Open file in O_RDONLY mode at client side
            6. Open with O_CREAT | O_EXCL | O_WRONLY mode at server side to create a file
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LIST-   In case of 'ls' command, it would create a temporary file and then transfers it to client . 
           So used readdir from <dirent.h> to get all files names using infinite while loop and append in string.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
QUIT- Simply checked for any file transfer and close the connection with the client.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DELE- 1. Open file in O_RDONLY to check does file exists.
           2. Used the system call to remove the desired file
           3. Prompt at client side to confirm deletion .
______________________________________________________________________________________________________________________________________________________
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

