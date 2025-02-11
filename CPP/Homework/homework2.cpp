#include <iostream>
#include <unistd.h>  // For pipe, fork, read, write
#include <cstring>   // For memset

int main() {
    int parentPipe[2], childPipe[2];
    pid_t fpid;
    int result = 1;

    if (pipe(parentPipe) < 0 || pipe(childPipe) < 0) {
        std::cout << "Pipe failed" << std::endl;
        return 1;
    }

    fpid = fork();
    if (fpid < 0) {
        std::cout << "Fork failed" << std::endl;
        return 1;
    }

    if (fpid == 0) {  
        close(childPipe[0]);  
        close(parentPipe[1]);  

        while (true) {
            read(parentPipe[0], &result, sizeof(result));  
            std::cout << "Child Recived : " << result << std::endl;
            result *= 2;
            sleep(1);
            write(childPipe[1], &result, sizeof(result));  
        }
    } else {  
        close(parentPipe[0]);  
        close(childPipe[1]);  

        write(parentPipe[1], &result, sizeof(result));  

        while (true) {
            read(childPipe[0], &result, sizeof(result));  
            std::cout << "Parent Recived : " << result << std::endl;
            sleep(1);
            write(parentPipe[1], &result, sizeof(result));  
        }
    }
}
