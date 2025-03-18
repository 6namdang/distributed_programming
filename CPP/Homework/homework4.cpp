#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>

int numNodes;
int targetNum;

void handle_child_signal(int sig) {
    std::cout << "Child process is terminating." << std::endl;
    exit(0);
}

int main() {
    std::cout << "How many nodes would you like in your ring?: ";
    std::cin >> numNodes;
    std::cout << "What number do you want to count to?: ";
    std::cin >> targetNum;
    std::cout << "Starting ring" << std::endl;

    int pipes[numNodes][2];
    for (int i = 0; i < numNodes; i++) {
        if (pipe(pipes[i]) == -1) {
            std::cerr << "Error creating pipe." << std::endl;
            exit(1);
        }
    }

    pid_t childPIDs[numNodes];
    for (int i = 0; i < numNodes; i++) {
        childPIDs[i] = fork();
        if (childPIDs[i] < 0) {
            std::cerr << "Error creating child process." << std::endl;
            exit(1);
        }

        if (childPIDs[i] == 0) {
            signal(SIGTERM, handle_child_signal);

            int prevNode;
            if (i == 0) {
                prevNode = numNodes - 1;
            } else {
                prevNode = i - 1;
            }
            
            int currentNumber;
            
            while (read(pipes[prevNode][0], &currentNumber, sizeof(currentNumber)) > 0) {
                std::cout << "Node " << getpid() << " received: " << currentNumber << " from pipe." << std::endl;
                
                if (currentNumber == targetNum) {
                    std::cout << "Reached our target: " << currentNumber << std::endl;
                    break;
                }
                
                currentNumber++;
                int nextNode = i;
                write(pipes[nextNode][1], &currentNumber, sizeof(currentNumber));
            }
            while (true) {
                pause();
            }
        }
    }

    int startValue = 1;
    write(pipes[0][1], &startValue, sizeof(startValue));
    sleep(1);

    std::cout << "I shall clean up my children." << std::endl;
    for (int i = 0; i < numNodes; i++) {
        kill(childPIDs[i], SIGTERM);
    }

    for (int i = 0; i < numNodes; i++) {
        waitpid(childPIDs[i], nullptr, 0);
    }

    std::cout << "All children have terminated. Parent is ending." << std::endl;
    return 0;
}
