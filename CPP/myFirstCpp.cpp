#include <iostream>//for terminal func

bool isEven(int end) {
    int even = 0;
    int odd = 0;
    for (int i=0; i < end; i++) {
        if (((i+1)%2)==0)
        {
            printf("%d is even \n", (i+1));
            even +=1;
        }
        else {
            printf("%d is odd \n", (i+1));
            odd += 1;
        }
    }
        if (even == odd) {
            return true;
        }
        else {
            return false;
        }
    }


int main() {
    printf("hello world \n");
    printf (" %d \n",isEven(10));
    

};
