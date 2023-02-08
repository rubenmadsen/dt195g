//
// Created by ruben on 2/8/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <wait.h>
#include <string.h>

void findStrInStr(char *text, char *str);
int main(int argc, char ** argv){
    bool isParent = false;
    FILE *fileptr;
    size_t fsize;
    fileptr = fopen("../bible.txt","r");
    if(!fileptr) {
        printf("Grande open problemas\n");
        exit(1);
    }
    fseek(fileptr,0,SEEK_END);
    fsize = ftell(fileptr);
    rewind(fileptr);
    printf("opened file (%zu bytes) ",fsize);
    size_t part = fsize/4;
    char parts[4][part];
    printf("parts will be %zu bytes each\n",part);
    for(int i=0; i<4; i++){
        char *partptr = parts[i];
        size_t bytes_read = fread(partptr,1,part,fileptr);
        if(bytes_read == -1){
            printf("Grande fread problemas\n");
            exit(1);
        }
        printf("Read %zu bytes\n",bytes_read);
    }
    char *ptr = parts[0];
    findStrInStr(ptr,"beginning");
    // start forking disaster
    for (int i = 0; i < 4; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // childs

            exit(0);
        }
    }
    // wait for morons
    for (int i = 0; i < 4; i++) {
        pid_t pid = wait(NULL);
        printf("Process %d is terminated.\n", pid);
    }
    return 0;
}
void findStrInStr(char *text, char *str){

    char *head = text;
    char *end = text+strlen(text);
    int row = 0;
    int col = 0;
    size_t len = strlen(str);
    char *tail = text+len;
    int stop = 0;
    head++;
    stop = 1;
        while(tail != end){
        if(strncmp(head,str,len) == 0){
            printf("Text found row %d col %d\n",row,col);
            return;
        }
        col++;
        if(head[0] == '\n'){
            row++;
            col = 0;
        }
        head++;
        tail++;
    }
    printf("Text not found\n");
}