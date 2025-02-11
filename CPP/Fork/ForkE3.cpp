// Online C++ compiler to run C++ program online
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
int summation(int start, int end){
    int loops = (end - start) + 1;
    int sum = 0;
    pid_t processPID = getpid();
    printf("pid : %d summing %d to %d\n", processPID, start, end);
    for (int i =0; i< loops; i++){
        sum = sum + (start + i);
        printf("process pid: %d sum %d\n", processPID, sum);
        sleep(2);
    }
    return sum;
}

int main() {
    printf("forking example 3, calculate the sum of a series 1-25 using 5 children \n");
    int sum = 0;
    pid_t fPid;
    fPid = fork();
    
    if (fPid == -1) {
        printf("fork failed \n");
        return 1;
    }
    else if(fPid == 0){
        sum = summation(1,5);
        exit(sum);
    }
    //child 2
        if (fPid == -1) {
        printf("fork failed \n");
        return 1;
    }
    else if(fPid == 0){
        sum = summation(6,10);
        exit(sum);
    }
    
    //3rd
    
    if (fPid == -1) {
        printf("fork failed \n");
        return 1;
    }
    else if(fPid == 0){
        sum = summation(11,15);
        exit(sum);
    }

    if (fPid == -1) {
        printf("fork failed \n");
        return 1;
    }
    else if(fPid == 0){
        sum = summation(16,20);
        exit(sum);
    }
    
    if (fPid == -1) {
        printf("fork failed \n");
        return 1;
    }
    else if(fPid == 0){
        sum = summation(21,25);
        exit(sum);
    }
    int value = 0;
    int status = 0;
    for (int i = 0; i< 5; i++) {
        printf("parent waiting for any child to stop counting \n");
        wait(&status);
        value = WEXITSTATUS(status);
        sum = sum + value;
        printf("return from child: %d the sum is now %d\n", value, sum);
        
    }
    printf("all children are done, the final sum is %d\n", sum);

    exit(sum);
}