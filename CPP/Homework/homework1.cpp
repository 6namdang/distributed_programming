#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
using namespace std;

int summation(int start, int end) {
    int loops = (end - start) + 1;
    int sum = 0;
    pid_t processPID = getpid();
    printf("pid : %d summing %d to %d\n", processPID, start, end);
    for (int i = 0; i < loops; i++) {
        sum = sum + (start + i);
        printf("process pid: %d sum %d\n", processPID, sum);
        sleep(2);
    }
    return sum;
}

int main() {
    int sum = 0;
    int number;
    int child;

    printf("We are going to calculate the sum of a series \n");
    cout << "Please provide a number" << endl;
    cin >> number;
    cout << "Please provide the number of children you wish to have calculate" << endl;
    cin >> child;

    int numPerChild = number / child;
    int remainder = number % child;
    cout << "Number of Children: " << child << " Number per process: " << numPerChild << " Remainder: " << remainder << endl;

    pid_t fpid;
    for (int i = 0; i < child; i++) {
        fpid = fork();

        if (fpid == -1) {
            printf("Child creation failed \n");
        }
        else if (fpid == 0) {
            int start = i * numPerChild + 1;
            int end = (i + 1) * numPerChild;
            int sum = summation(start, end);
            exit(sum);  
        }
    }

    //if remainder aint equal to 0 then i create 1 more to handle the rest
    if (remainder != 0) {
        fpid = fork();
        if (fpid == -1) {
            printf("Child creation failed! \n");
        }
        else if (fpid == 0) {
            
            int start = child * numPerChild + 1;
            int end = start + remainder - 1;  
            int sum = summation(start, end);
            exit(sum);  
        }
    }

    int value = 0;
    int status = 0;
    for (int i = 0; i < child + (remainder != 0); i++) {
        printf("parent waiting for any child to stop counting \n");
        wait(&status);
        value = WEXITSTATUS(status);
        sum = sum + value;
        printf("return from child: %d the sum is now %d\n", value, sum);
    }

    printf("all children are done, the final sum is %d\n", sum);
    exit(sum);
};
