#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>
#define MAX_SIGNALs 64

const int numOfChildren = 5;
int deadChildren =0;
pid_t children[numOfChildren];
bool unstoppable = true;

void handle_sigint(int sig) {
    std::cout << "ha ha , you cant stop me" << std::endl;
}

void handle_sigterm_parent(int sig)
{
    printf("i shall clean up my children \n");
    for (int i=0; i< numOfChildren; i++) {
        kill(children[i], SIGTERM);
    }
    while (deadChildren != numOfChildren) {
        std::cout << "parent wait for child to die" << deadChildren << std::endl;
        sleep(1);
    }
    printf("all children dead, ending with parent \n");
    exit(0);

}
void handle_sigterm_child(int sig) {
    std::cout << "oh no you have found my only weakness, asking nicely" << std::endl;
    sleep(5);
    unstoppable = false;
}
void handle_sigchld(int sig) {
    deadChildren = deadChildren + 1;
}

int main() {
    
    //block all blockablae singnals by defaukt
    for (int i=0; i<=MAX_SIGNALs; i++) {
        if (signal(i, SIG_IGN) == SIG_ERR) {
            std::cout << "Could not block signal" << std::endl;
        }
    }
    signal(SIGINT, handle_sigint);

    //make our children:

    for (int i=0; i<numOfChildren; i++){
        children[i] = fork();
        if (children[i] == -1) {
            std::cout << "fork failed" << std::endl;
            exit(1);
        }
        else if(children[i] == 0) {
            //child process
            signal(SIGTERM, handle_sigterm_child);
            
            int n=0;
            pid_t pPid = getpid();
            while(unstoppable) {
                n = n+1;
                std::cout << "child pid" <<pPid << "misssispi" << n<< std::endl;
                sleep(1);
            }
            printf("child is out of loop \n");
            exit(0);
        }
    }
    
}
    //process that should get this far.
