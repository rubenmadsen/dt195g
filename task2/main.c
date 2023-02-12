//
// Created by ruben on 2/8/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/resource.h>
#include <time.h>

unsigned int findStrInStr(char *head, char *end, char *str);
int main(int argc, char ** argv){
    //uint64_t *shared_results = mmap(NULL,facs*sizeof(uint64_t),PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    struct rlimit limit;
    getrlimit (RLIMIT_STACK, &limit);
    printf ("\nStack Limit = %ld and %ld max\n", limit.rlim_cur, limit.rlim_max);
    bool isParent = false;
    //
    clock_t tos = clock();
    unsigned int *total;
    total = malloc(sizeof(unsigned int));
    *total = 0;
    FILE *fileptr;
    size_t fsize;
    fileptr = fopen(argv[1],"r");
    if(!fileptr) {
        printf("Grande open problemas\n");
        exit(1);
    }
    fseek(fileptr,0,SEEK_END);
    fsize = ftell(fileptr);
    rewind(fileptr);
    printf("opened file (%zu bytes) ",fsize);
    size_t part = fsize/4;
    char bytes[fsize];
    fread(bytes,1,fsize,fileptr);
    printf("Searching '%s' for '%s'\n",argv[1],argv[2]);
//    findStrInStr(bytes,bytes+part,argv[2]);
//    findStrInStr(bytes,bytes+part,argv[2]);

    // start forking disaster
    for (int i = 0; i < 4; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // childs
            *total += findStrInStr(bytes+(part*i),bytes+part+(part*i),argv[2]);

            exit(0);
        }
    }
    // wait for morons
    for (int i = 0; i < 4; i++) {
        pid_t pid = wait(NULL);
        printf("Process %d is terminated.\n", pid);
    }
    printf("Found: %d results\n",*total);
    clock_t toe = clock();
    double elapsed_time = (double)(toe - tos) / CLOCKS_PER_SEC;
    printf("Total time: %f ms\n", elapsed_time);
    return 0;
}
unsigned int findStrInStr(char *head, char *end, char *str){
    clock_t start = clock();
    unsigned int found = 0;
    //char *head = text;
    //char *end = text+strlen(text);
    int row = 0;
    int col = 0;
    pid_t main = getpid();
    size_t len = strlen(str);
    char *tail = head+len;

        while(tail != end) {
            if (strncmp(head, str, len) == 0) {
                //printf("Text found row %6d col %3d\n", row, col);
                //return;
                found++;
            }
            col++;
            if (head[0] == '\n') {
                row++;
                col = 0;
            }
            head++;
            tail++;
        }
    //printf("found %d\n",found);
    clock_t ending = clock();
    double elapsed_time = (double)(ending - start) / CLOCKS_PER_SEC;
    printf("Elapsed time: %fs\n", elapsed_time);
    return found;
    //printf("Text not found\n");
}