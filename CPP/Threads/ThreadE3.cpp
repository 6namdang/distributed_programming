#include <iostream>
#include <thread>
#include <sys/wait.h>
#include <vector>

using namespace std;
int globalNum = 0;

void addingToGlobal(int threadNum, int add) {
    cout << "thread number" <<  threadNum << "adding" << add << "to" <<globalNum << endl;
    for (int i=0; i < add; i ++)
    {   globalNum = globalNum + 1;
        cout << "Thread number" << threadNum << "count "<< globalNum << endl;
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

int main () {
    int numOfThreads = 10000;
    vector<thread> globalThreads;

    for (int i=0; i< numOfThreads; i++) {
        globalThreads.emplace_back(addingToGlobal, i, 5);
    }
    int localNum = 0;
    vector<thread> pointerThreads;
    for (int i=0; i< numOfThreads; i++) {
        pointerThreads.emplace_back(addingWithPointers, i + numOfThreads, 5, &localNum);
    }

    
    for (thread& th : globalThreads) {
        th.join();
    }

    for (thread& th : pointerThreads) {
        th.join();
    }
    cout << "Global Value" << globalNum << endl;
    cout << "Local value " << localNum << endl;
    cout << "Expected value" << 5*numOfThreads << endl;

}
