#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <unistd.h>

using namespace std;

int totalSum = 0;
mutex sumMutex;

void summation(int threadNum, int start, int end)
{
    int localSum = 0;
    cout << "Thread: " << threadNum << " summing " << start << " to " << end << endl;

    for (int i = start; i <= end; i++)
    {
        localSum += i;
        cout << "Thread: " << threadNum << " sum " << localSum << endl;
        sleep(2);
    }

    {
        lock_guard<mutex> lock(sumMutex);
        totalSum += localSum;

        cout << "Returned from thread: " << threadNum << " The sum is now " << totalSum << endl;
    }
}

int main()
{
    int number, children;

    cout << "We are going to calculate the sum of a series" << endl;
    cout << "Please provide a number" << endl;
    cin >> number;
    cout << "Please provide the number of children you wish to have calculate" << endl;
    cin >> children;

    int numPerChild = number / children;
    int remainder = number % children;

    cout << "Number of Children: " << children << " Number per Process: " << numPerChild << " remainder " << remainder << endl;

    vector<thread> threads;

    for (int i = 0; i < children; i++)
    {
        int start = i * numPerChild + 1;
        int end = (i + 1) * numPerChild;
        threads.emplace_back(summation, i, start, end);
    }

    if (remainder != 0)
    {
        int start = children * numPerChild + 1;
        int end = start + remainder - 1;

        cout << "We have a remainder of: " << remainder << " Spawning extra child" << endl;
        threads.emplace_back(summation, children, start, end);
    }

    for (thread &th : threads)
    {
        th.join();
    }

    cout << "All children are done, the final sum is " << totalSum << endl;
    return 0;
}