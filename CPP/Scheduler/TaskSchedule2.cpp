#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

int main() {
    uint32_t n =0;
    uint64_t c =0;
    double calculatedTime;
    pid_t pPid = getpid();

    printf("PID %d\n", pPid);
    time_t startTime = time(NULL);
    while (true) {
        if(n == 2147483648) {
            calculatedTime = difftime(time(NULL), startTime);
            c = c+1;
            printf("%lu %.2f to count to %u \n",c,calculatedTime,(uint32_t)(n-1));
            n=0;
            sleep(20);
            startTime = time(NULL);
              
        }
        n = n+1;
    }
}