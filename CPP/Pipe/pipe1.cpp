// Online C++ compiler to run C++ program online
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>

int main() {
    char text[100] = "hello world!";
    int fd[2];
    
    int pipeReturn = pipe(fd);
    if(pipeReturn == -1) {
        printf("pipe creation failed \n");
        exit(1);
    }
    
    pid_t fPid = fork();
    if (fPid == -1) {
        printf("Fork failed \n");
        exit(1);
    }
    else if(fPid == 0){
        //child process 
        close(fd[1]); //child will only read, close write end.
        
        //use the read command to read from the read end of the pipe
        printf("child is waiting for data to be written to pipe \n");
        ssize_t bytesRead = read(fd[0], text, sizeof(text));
        
        if(bytesRead < 0) {
            printf("reading failed \n");
            exit(1);
            
        }
        printf("child read the following from pipe : %s\n", text);
        close(fd[0]);
        exit(0);
        
        
        
    }
    else {
        //parent process
        close(fd[0]); //parent will only write,
        strcpy(text, "hello from parent");
        printf("in 5 seconds parent will send %s\n", text);
        sleep(5);
        ssize_t bytesWritten = write(fd[1], text, 21);
        if (bytesWritten < 0) {
            printf("writing failed \n");
        exit(1);
        }
        close(fd[1]);
        wait(NULL);
        
    }


    return 0;
}