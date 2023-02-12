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
#include <sys/mman.h>
#include <time.h>
#include <math.h>

unsigned int *shared_total;

unsigned int findStrInStr(char *head, char *end, char *str);
int main(int argc, char **argv){
    shared_total  = mmap(NULL,sizeof(unsigned int),PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);
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

    // start forking disaster
    for (int i = 0; i < 4; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // childs
            int extended = (int)ceil((double)strlen(argv[2])/2);
            char *head = bytes+(part*i);
            char *end = bytes+part+(part*i) + extended;
            *shared_total += findStrInStr(head,end,argv[2]);

            exit(0);
        }
    }
    // wait for morons
    for (int i = 0; i < 4; i++) {
        pid_t pid = wait(NULL);
        //printf("Process %d is terminated.\n", pid);
    }
    printf("\nFound: %d results\n",*shared_total);
    return 0;
}
unsigned int findStrInStr(char *head, char *end, char *str){
    unsigned int found = 0;
    int row = 0;
    int col = 0;
    pid_t main = getpid();
    size_t len = strlen(str);
    char *tail = head+len;

        while(tail != end) {
            if (strncmp(head, str, len) == 0) {
                printf("Text found row %6d col %3d\n", row, col);
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
        return found;
}