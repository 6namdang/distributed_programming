#include <iostream>
#include <thread>
#include <sys/wait.h>
#include <mutex>

class Bank {
    private:
    int accountNumber;
    int balance;
};

