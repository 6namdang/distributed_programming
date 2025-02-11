#include <sys/wait.h> //sleep
#include <iostream> //for terminal
#include <unistd.h> //for fork(), getpid(), getppid()

int main() {
    printf("this is fork \n");
    pid_t fpid = fork(); //fork is when we duplciate our current process
    pid_t processPid = getpid();
    if (fpid == -1){
        printf("forked failed \n");
        return 1;
    }
    else if(fpid > 0 ) {
        //parent process
        while(true){
            printf("i am the parent my pid is : %d and my child pid is %d\n", processPid, fpid);
            system("ps");
            sleep(4);
        }
    }
        else {
            pid_t parrentPid = getppid();
            while(true){
                printf("i am the child, my pid is %d, fork returned %d, my parent pid is %d\n", processPid, fpid, parrentPid );
                sleep(2);
            }
        }
        exit(0);
    }

    
