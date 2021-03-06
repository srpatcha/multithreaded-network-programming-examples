#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "Common.h"

// Compilation:
// gcc "03 - Nonamed_pipes_read_write.c" -o nonamedpipesrw

// Task:
// Do write on one end and read on another end.

int main()
{
    int     fd[2], nbytes;
    pid_t   childpid;
    char    string[] = "Hello, world!\n";
    char    readbuffer[80];
    
    // Handle child process killing.
    handle_child_finishing();
    
    pipe(fd);
    
    printf("Process %d creates child\n", getpid());
            
    if((childpid = fork()) == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if(childpid == 0)
    {
        // Since descriptors are shared between the parent and child,
        // we should always be sure to close the end of pipe we aren't concerned with.
        // Child process closes up input side of pipe
        close(fd[0]);

        // Send "string" through the output side of pipe
        write(fd[1], string, (strlen(string)+1));
        printf("Done write in child process %d\n", getpid());
    }
    else
    {
        // Parent process closes up output side of pipe
        close(fd[1]);

        // Read in a string from the pipe
        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        printf("Received string: %s in process %d\n", readbuffer, getpid());
    }
    
    printf("End process %d\n", getpid());
    
    exit(0);
}