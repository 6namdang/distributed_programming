#include <iostream>

#include <thread>
#include <sys/wait.h>


using namespace std;
int globalNum = 5;
void addingToGlobal(int threadNum, int add) {
    cout << "thread number" <<  threadNum << "adding" << add << "to" <<globalNum << endl;
    for (int i=0; i < add; i ++)
    {   globalNum = globalNum + 1;
        cout << "Thread number" << threadNum << "count "<< globalNum << endl;
        sleep(2);
    }
    cout << "Thread " << threadNum << "Has finished counting to " << globalNum << endl;

}

void addingWithPointers(int threadNum, int add, int *num) {
    cout << "thread number" <<  threadNum << "adding" << add << "to" << *num << endl;
    for (int i=0; i < add; i ++)
    {   *num = *num + 1;
        cout << "Thread number" << threadNum << "count "<< *num << endl;
        sleep(2);
    }
    cout << "Thread " << threadNum << "Has finished counting to " << *num << endl;
   
}

int main() {
    thread globalThread(addingToGlobal, 1, 5);
    int localNum =5;
    thread pointerThread(addingWithPointers, 2, 5, &localNum);
    globalThread.join();
    pointerThread.join();

    cout << "global value" << globalNum << "local value" << localNum << endl;
}