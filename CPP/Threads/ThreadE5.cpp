#include <iostream>
#include <thread>
#include <mutex>
#include <sys/wait.h>

using namespace std;

void slomNumberChaner(int threadNum, int *number, mutex *numberMutex)
{
    int x;
    for (int i = 0; i < 5; i++)
    {
        numberMutex->lock();
        x = *number;
        x += 1;
        sleep(2);
        *number = x;
        numberMutex->unlock();
        printf("We have increased myNumber by 1 || \n");
        sleep(2);
    }
}

int main()
{
    int myNumber = 0;
    mutex myMutex;
    thread thread1(slomNumberChaner, 1, &myNumber, &myMutex);
    sleep(1);

    thread thread2(slomNumberChaner, 2, &myNumber, &myMutex);
    thread1.join();
    thread2.join();

    cout << "Number" << myNumber << endl;
    return 0;
}