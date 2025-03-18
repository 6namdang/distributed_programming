#include <iostream>

int tradadd1(int n) {
    n++;
    return n;
}
void add1(int n) {
    n++;
}
void add1(int* n) {
    *n = *n+1;
}
int main() {
    int num=0;
    num = tradadd1(num); 
    printf("trad add 1 %d\n", num);
    int* ptr = &num;
    add1(ptr);
    printf("add a pointer %d\n", num);
}