/*
@course:     EECS 4740 OS
@assignment: Lab 2
@author:     Tamara Linse
@date:       9.15.2022
@file:       filecopy.cpp
@purpose:    Designing and implementing a file-copying program using ordinary pipes.
*/

#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define BUFFER_SIZE 25  // Read buffer size
#define READ_END 0      // Read end of the pipe used in the file descriptor array
#define WRITE_END 1     // Write end of the pipe used in the file descriptor array

using namespace std;

/*
* @Parem argc: Number of passed arguments
* @Parem argv: Values of passed arguments
*/

int main(int argc, char** argv)
{
    /*
    *   ======================= TODO 1 =======================
    *
    *   Check that the correct number of arguments have been passed (3).
    *   If not, print a short usage message and return 1.
    *   Note: argc will always be >= 1 as the executable name is always passed as an argument.
    */

    if (argc < 1) {
        cout << "Incorrect number of arguments passed." << endl;
        return 1;
    }

    /*
    *   Initializing Variables
    *   Variable used by both parent and child to track the number of bytes read or written.
    */

    int numRead;
    char write_buffer[BUFFER_SIZE]; // Write buffer for input file and pipe.
	char read_buffer[BUFFER_SIZE];  // Read buffer for input file and pipe.
	int fd[2];                      // File descriptor array used to create a pipe.

    /*
    *   ======================= TODO 2 =======================
    *
    *   Create a pipe with an appropriate error check using the file descriptor array above.
    *   If the pipe failed print a short error message and return 1.
    */

    if(pipe(fd) == -1)
    {
        cerr << "Pipe creation failed." <<  endl;
        return 1;
    }

	/*
    *   ======================= TODO 3 =======================
    *
    *   Fork the child process.
    *   You should have three cases to check after the fork:
    *       The fork fails.
    *       The parent process is executing.
    *       The child process is executing.
    */

    pid_t pid = fork();
    
    if(pid < 0)
    {
        cerr << "Fork creation failed." << endl;
        return 1;
    }

    /*
    *   ======================= TODO 4 =======================
    *
    *   Implement code for the parent process.
    *   Note: this will be INSIDE the fork if statement for the parent process.
    * 
    *   ======================= TODO 4.1 =======================
    *
    *   Close the read end of the pipe using the 'close' function.
    *   Open the input file using the 'fopen' function in read mode to read its contents.
    *       For example: FILE* fp = fopen("input.txt", "r");
    * */

    if(pid > 0)
    {

        // Parent Process

        // Close the read end of the pipe.
        close(fd[READ_END]);

        // Open input file.
        FILE* fp = fopen("input.txt", "r");



    /*
    *   ======================= TODO 4.2 =======================
    *
    *   Create a loop that will:
    *       Write the contents of the input file to the read buffer.
    *       Write the contents of the read buffer to the pipe.
    *       Continue until all the contents of the input file have been written to the pipe.
    *   Use the function fread(void *ptr, size_t size, size_t nmemb, FILE *stream)
    *   to read the file bytes.
    *       *ptr: Buffer to write read contents
    *       size: Size in bytes of each element to read [sizeof(*read_buffer)]
    *       nmemb: Number of elements to read
    *       *stream: File pointer to read from
    *   Hint: numRead should store the return of fread(...)
    *   which is the number of bytes read from the file.
    */

        if (! fp) {

            cout << "Unable to open input file." << endl;
            return 0;

        } else {

            do {

                // Writing file to read buffer.
                numRead = fread(read_buffer, sizeof(read_buffer), strlen(read_buffer), fp);

                // Writing read buffer to pipe.
                write(fd[WRITE_END], write_buffer, strlen(write_buffer)+1); 

            } while (numRead != 0);
        }

    /*
    *   ======================= TODO 4.3 =======================
    *
    *   Close the write end of the pipe.
    *   Close the file pointer using the 'fclose' function.
    *   Wait for the child process to finish.
    *   Return 0 when the child finishes.
    */

        // Close the write end of the pipe.
        close(fd[WRITE_END]);

        // Close the file.
        fclose(fp);

        // Wait for child process to finish and then return 0.
        wait(NULL);
        cout << "Child has finished. Parent is exiting." << endl;
        return 0;

    }

    /*
    *   ======================= TODO 5 =======================
    *
    *   Implement code for child process.
    * 
    *   ======================= TODO 5.1 =======================
    *
    *   Close the write end of the pipe.
    *   Open the output file in write mode to write the copied contents.
    */

    else
    {
        // Child Process

        // Close the write end of the pipe.
        close(fd[WRITE_END]);

        // Open output file in write mode.
        FILE* fp = fopen("output.txt", "w");


    /*
    *   ======================= TODO 5.2 =======================
    *
    *   Create a loop that will:
    *       Write the contents of the pipe to the read buffer.
    *       Write the contents of the read buffer to the output file.
    *       Continue until all the contents of the pipe have been written to the output file.
    *   Use the function fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream)
    *   to write the bytes to the file.
    *       *ptr: Buffer to read from
    *       size: Size in bytes of each element to write [sizeof(*read_buffer)]
    *       nmemb: Number of elements to write
    *       *stream: File pointer to write to
    *   Hint: numRead should store the number of bytes read from the pipe.
    */


        if (! fp) {
            cout << "Unable to open output file." << endl;
            return 0;
        } else {

            while (&fd[READ_END] != NULL) {

                // Write pipe to read buffer.
                read(fd[READ_END], read_buffer, BUFFER_SIZE);

                // Writing buffer to output file.
                numRead = fwrite(write_buffer, sizeof(write_buffer), strlen(write_buffer), fp);

            }
        }

    /*
    *   ======================= TODO 5.3 =======================
    *
    *   Close the read end of the pipe and file pointer
    *   Return 0 when the child process finishes.
    */

        // Close the read end of the pipe.
        close(fd[READ_END]);

        // Close the file.
        fclose(fp);
        
        return 0;
    } 

}
