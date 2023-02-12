#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
uint64_t factorial(int f){
    uint64_t result = 1;
    uint64_t sum = 0;
    for (int i=1; i<=f;i++) {
        result *= i;
        sum += result;
        printf("%lu ", result);
    }

    printf("\n");
    return sum;
}
int main(int argc, char* args[]) {

    int facs = atoi(args[1]);
    if(facs <= 0){
        printf("%s <positive number>\n",args[0]);
        exit(-1);
    }
    pid_t id = fork();
    uint64_t sums[facs];
    //sums[0] = 1;
    if(id == 0){
        uint64_t sum = 0;
        for(int i=1; i<=facs; i++){
            sums[i] = factorial(i);
            //printf("[SUM]=%lu |",sums[i-1]);
        }
        printf("\n");
        printf("The sum of the series is\n\n");
        for(int i=1; i<=facs; i++){
            printf("%lu ",sums[i]);
        }
        printf("\n");

        exit(0);
    }
    wait(NULL);
    return 0;
}
