#include <sys/wait.h> //sleep
#include <iostream> //for terminal
#include <unistd.h> //for fork(), getpid(), getppid()

int main() {
int count = 0;
pid_t fpid = fork();
if (fpid == -1){
    printf("forked failed");
    return -1;
}
else if( fpid > 0){
    printf("parent waiting for child to stop counting \n");
    int status;
    wait(&status); // our parent is going to wait for any child process to finish. 
    printf("parent no longer waiting \n");
    int childExitStatus = WEXITSTATUS(status);
    printf("parent count: %d child count from status: %d\n", count, childExitStatus);
    exit(0);
//parent process
}
else {
    //child process
    printf("i am the child i am going to count to 5\n");
    while ( count <5){
        count +=1;
        printf("count %d\n", count);
        
        sleep(2);
        }
    printf("i child am done couting, returning count: %d\n", count);
    exit(count);
        }
    exit(0);
}