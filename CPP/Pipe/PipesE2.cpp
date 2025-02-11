#include <sys/wait.h> //sleep and wait
#include <iostream>   //for terminal
#include <unistd.h>   // fork() getpip() read() write() pipe()
#include <stdlib.h>   // read and srand()
#include <time.h>     // for time()

// program that has multiple children recieving and processing data made by a single parent

int calPower(int n1, int n2){
    pid_t processPID = getpid();

    int answer = n1;
    for(int i = 0; i < n2; i++){
        answer = answer * n1;
        printf("child %d calculating %d^%d: %d\n",processPID, n1, n2, answer);
        sleep(1);
    }

        printf("Done %d calculating %d^%d: %d\n",processPID, n1, n2, answer);
    return answer;
}


int main()
{

    // set our seed
    srand((unsigned int)time(NULL));

    int numOfCalculations = (rand() % 50) + 1;
    printf("Number of sums to calculate: %d\n", numOfCalculations);

    int numOfChildren = (rand() % 10) + 1;
    printf("Number of children: %d\n", numOfChildren);

    int childPipe[2];
    int pipReturn = pipe(childPipe);

    if (pipReturn == -1)
    {
        printf("pipe Creation faild\n");
        exit(1);
    }

    pid_t fpid;

    for (int i = 0; i < numOfChildren; i++)
    {
        fpid = fork();
        if (fpid == -1)
        {
            printf("pipe Creation faild\n");
            exit(1);
        } else if(fpid == 0){
            //child process
            close(childPipe[1]);
            pid_t processPid = getpid();
            int nums[3];


            ssize_t bytesRead;
            while(true){
                //read from the pip
                bytesRead = read(childPipe[0], nums, sizeof(nums));
                if(bytesRead < 0){
                    printf("Pipe read faild\n");
                    exit(1);
                }else if(bytesRead == 0){
                    //no more bytes in the array
                    printf("child %d sees no more in pipe, ending\n", processPid);
                    exit(0);
                }
                printf("Child %d read the follwing from pipe(%d, %d ,%d)\n", processPid, nums[0],nums[1],nums[2]);
                calPower(nums[0],nums[i]);

            }
        }
    }
    //parent process
    close(childPipe[0]);

    //generate data 
    int calculation[numOfCalculations][3];

    for(int i = 0; i < numOfCalculations; i++ ){
        calculation[i][0] = (rand() % 20) +1;
        calculation[i][1] = (rand() % 20) +1;
        calculation[i][2] = i;
    }
ssize_t bytesWritten;
    for(int i = 0; i < numOfCalculations; i++){
        bytesWritten =  write(childPipe[1], calculation[i], sizeof(calculation));
        if(bytesWritten != sizeof(calculation[i])){
        printf("pipe write faild\n");
        exit(1);
        }
        printf("parent wrote: (%d, %d, %d)\n", calculation[i][0], calculation[i][0], calculation[i][2]);
    }

    close(childPipe[1]);

    // waiting for all children to finish
    for(int i = 0; i < numOfChildren; i++){
        wait(NULL);
    }

    return 0;
}
