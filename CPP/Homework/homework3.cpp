#include <sys/wait.h> 
#include <iostream>  
#include <unistd.h>  
#include <stdlib.h>   
#include <time.h>     
#include <math.h>     
#include <stdio.h>    

using namespace std;

int calPower(int n1, int n2) {
    pid_t processPID = getpid();
    int answer = n1;
    // Multiply n1 by itself (n2-1) times
    for (int i = 1; i < n2; i++){
        answer = answer * n1;
        printf("child %d calculating %d^%d: %d\n", processPID, n1, n2, answer);
        sleep(1);
    }
    printf("%d Done %d^%d: %d \n", processPID, n1, n2, answer);
    return answer;
}

int main()
{
    // Introductory text.
    int numOfCalculations, numOfChildren, seed;
    
    cout << "Please provide number of calculations you would like to calculate:" << endl;
    cin >> numOfCalculations;
    cout << "Please provide the number of children you wish to have calculate" << endl;
    cin >> numOfChildren;
    cout << "Please provide a seed" << endl;
    cin >> seed;

    srand(seed);

    int childPipe[2];
    int parentPipe[2];
    
    if (pipe(childPipe) == -1) {
        perror("pipe creation failed");
        exit(1);
    }
    if (pipe(parentPipe) == -1) {
        perror("pipe creation failed");
        exit(1);
    }
    
    // Fork off the children.
    pid_t fpid;
    for (int i = 0; i < numOfChildren; i++){
        fpid = fork();
        if (fpid < 0) {
            perror("fork failed");
            exit(1);
        } else if (fpid == 0) {
            // CHILD PROCESS
            close(childPipe[1]);  
            close(parentPipe[0]);  
            
            pid_t processPid = getpid();
            int nums[3];  
            ssize_t bytesRead;
            
            while (true) {
                bytesRead = read(childPipe[0], nums, sizeof(nums));
                if (bytesRead < 0) {
                    printf("child %d pipe read failed\n", processPid);
                    exit(1);
                } else if (bytesRead == 0) {
                    printf("child %d sees no more in pipe, ending\n", processPid);
                    break;
                }
               
                if (bytesRead != sizeof(nums)) {
                    printf("child %d read incomplete data\n", processPid);
                    exit(1);
                }
                printf("Child %d read the following from pipe(%d, %d ,%d)\n", processPid, nums[0], nums[1], nums[2]);
                int result = calPower(nums[0], nums[1]);
                ssize_t bytesWritten = write(parentPipe[1], &result, sizeof(result));

                if (bytesWritten != sizeof(result)) {
                    printf("child %d write to parent pipe failed\n", processPid);
                } else {
                    printf("%d wrote %d to parent pipe\n", processPid, result);
                }
            }
            close(childPipe[0]);
            close(parentPipe[1]);
            exit(0);
        }
    }
    close(childPipe[0]);   
    close(parentPipe[1]);  
    
    int calculations[numOfCalculations][3];
    
    for (int i = 0; i < numOfCalculations; i++ ) {
        calculations[i][0] = (rand() % 20) + 1; 
        calculations[i][1] = (rand() % 20) + 1;  
        calculations[i][2] = i;                  
    }
    
    ssize_t bytesWritten;
    for (int i = 0; i < numOfCalculations; i++){
        bytesWritten = write(childPipe[1], calculations[i], sizeof(calculations[i]));
        if (bytesWritten != sizeof(calculations[i])){
            printf("pipe write failed\n");
            exit(1);
        }
        printf("parent wrote: (%d, %d, %d) to the child pipe\n", calculations[i][0], calculations[i][1], calculations[i][2]);
    }
    close(childPipe[1]);
    
    int expectedSum = 0;
    for (int i = 0; i < numOfCalculations; i++){
        int value = (int) round(pow(calculations[i][0], calculations[i][1]));
        expectedSum += value;
    }
    printf("Parent calculated expected sum of exponents: %d\n", expectedSum);
    
    int sumFromChildren = 0;
    int childResult;
    while (true){
        ssize_t r = read(parentPipe[0], &childResult, sizeof(childResult));
        if (r < 0){
            printf("parent pipe read failed\n");
            exit(1);
        } else if (r == 0){
            printf("parent sees no more in parent pipe, ending \n");
            break;
        }
        sumFromChildren += childResult;
        printf("Parent read %d from parent pipe, sumFromChildren is now %d\n", childResult, sumFromChildren);
    }
    printf("Parent pipe is empty, pre calculated sum: %d sumFromChildren: %d\n", expectedSum, sumFromChildren);
    close(parentPipe[0]);
    
    for (int i = 0; i < numOfChildren; i++){
        wait(NULL);
    }
    
    return 0;
}
