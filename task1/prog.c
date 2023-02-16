//
// Created by ruben on 2/6/23.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_INPUT 64
#define MAX_HISTORY 10
#define PROMPT printf("osh>")
// typedef
typedef struct H_item H_item;
struct H_item {
    char *arg;
    H_item *next;
};

//
void history(char *arg);

char *getHistoryItem(int *index);

void clearHistory();

// Global
H_item *head = NULL;
H_item *tail = NULL;
int historyCounter = 0;

int main(int argc, char **argv) {
    char input[MAX_INPUT];
    bool shouldWait = true;
    pid_t id;
    do {
        unsigned int count = 0;
        // Get input and make strtok backup
        char **args;
        PROMPT;
        fgets(input, MAX_INPUT, stdin);
        //
        //

        // !
        if (input[0] == '!') {
            char *short_input = strtok(input, " ");
            if (input[1] == '!') {
                //DONE Get history number n
                char num = 0;
                int h_val = (int) strtol(&num, NULL, 10);
                h_val--;
                char *h = getHistoryItem(&h_val);
                if(h == NULL){
                    printf("History does not exist.\n");
                    continue;
                }
//                printf("History: %d %s\n",h_val+1,h);
                strcpy(input, h);
            } else {
                //DONE Get history number n
                char *num = &short_input[1];
                int h_val = (int) strtol(num, NULL, 10);
                h_val--;
                char *h = getHistoryItem(&h_val);
                if(h == NULL){
                    printf("History does not exist.\n");
                    continue;
                }
//                printf("History: %d %s\n",h_val+1,h);
                strcpy(input, h);
            }
        }
        // History
        else if (input[0] == 'h' || strncmp(input, "history", 7) == 0) {
            printf("history\n");
            H_item *cur = head;
            int index = 1;
            while (cur != NULL) {
                printf("%d %s\n", index, cur->arg);
                cur = cur->next;
                index++;
            }
            continue;
        }
            // InputTas


            if (input[strlen(input) - 1] == '\n') {
                input[strlen(input) - 1] = '\0';
            }
            char input_str[MAX_INPUT];
            strcpy(input_str, input);
            history(strdup(input_str));
            //unsigned int count = 0;
            char *arg = strtok(input_str, " ");
            while (arg != NULL) {
                count++;
                arg = strtok(NULL, " ");
            }
            count++;
            char *ampersand = strstr(input, "&");
            if (ampersand == NULL) {
                shouldWait = true;
            }
            else {
                shouldWait = false;
                count--;
            }

            args = malloc(sizeof(char *) * count);
            arg = strtok(input, " ");
            unsigned int c = 0;
            while (arg != NULL) {
                if(strncmp(arg,"&",1) == 0) {
                    arg = strtok(NULL," ");
                }else{
                    args[c] = calloc(strlen(arg), sizeof(char));
                    strncpy(args[c], arg, strlen(arg));
                    arg = strtok(NULL, " ");
                    c++;
                }

            }
            args[c] = NULL;
            for (int i = 0; i < count; i++) {
                //printf("%d -> [%s]\n",i, args[i]);
            }

        id = fork();
        if (id == -1) {
            exit(EXIT_FAILURE);
        }
        //Child
        if (id == 0) {
//            for (int i = 0; i < count; i++) {
//                printf("%s ", args[i]);
//            }
//            printf("\n");
            execvp(args[0], ((char *const *) args));
            exit(EXIT_SUCCESS);
        }
        // Parent
        else{
            if (shouldWait == true) {
                printf("WAITING\n");
                wait(NULL);
            }
            else{
                printf("NOT WAITING\n");
            }
        }
        for (int i = 0; i < count; i++) {
            free(args[i]);
            args[i] = NULL;
        }
        free(args);
        args = NULL;
        historyCounter++;
    } while (strncmp(input, "quit", 4) != 0);


    clearHistory();
    return 0;
}

void history(char *arg) {
    H_item *new = malloc(sizeof(H_item));
    new->arg = arg;
    new->next = NULL;
    if (head == NULL) {
        head = new;
        tail = new;
        return;
    }
    tail->next = new;
    tail = new;
    historyCounter++;
}

char *getHistoryItem(int *index) {
    if(head == NULL)
        return NULL;

    int target = *index;
    H_item *cur = head;
    for (int i = 0; i < target; i++) {
        if (cur == NULL)
            return NULL;
        cur = cur->next;
        *index = i;
    }
    return cur->arg;
}

void clearHistory() {
    H_item *cur = head;
    head = NULL;
    while (cur != NULL) {
        free(cur->arg);
        H_item *before = cur;
        cur = cur->next;
        free(before);
    }
}
