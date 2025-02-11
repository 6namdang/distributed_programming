#include <iostream>
#include <csignal>
#include <sys/wait.h>
#include <unistd.h>

int count = 0;
bool mMad = false;
void handleSIGUSER1(int sig) {
    count = count + 1;
    printf("poke received %d\n", count);
    if (count >=5) {
        mMad == true;
    }
}

int main() {
    signal(SIGUSR1, handleSIGUSER1);//define the function to call when receiving SIGUSR1

    pid_t fpid = fork();
    pid_t ppid = getpid();

    if (fpid < 0) {
        printf("failed \n");
    }
    
    else if (fpid > 0) 
    {
        while (mMad == false) {
            sleep(5);
        }

    }
    else
    {
        printf("i am the child, i am going to poke the mother \n");
        pid_t mpid = getpid();
        while (true) {
            printf("poking mother \n");
            kill(mpid, SIGUSR1);
        }
    }

}
    
