#include <iostream>
#include <thread>
#include <sys/wait.h>

using namespace std;

void threadfunc(int countTo) {
    for (int i=0; i< countTo; i++) {
        cout << " count from " << i << " to " << countTo << endl; 
    }
}


int main() {
    thread myThread(threadfunc, 10);
    printf("Threads example 1 \n");
    myThread.join();
    myThread.detach();

    printf("Creating thread 2");
    thread thread2([](){
        cout << "declaring my own thread function "<< endl;
    });
}