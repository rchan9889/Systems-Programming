#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include "mymalloc.c"

#ifndef REALMALLOC
#include "mymalloc.h"
#endif

int main(int argc, char **argv){
    struct timeval start;
    struct timeval fin;
    struct timezone est;
    gettimeofday(&start, &est);
    for(int i = 0; i < 50; i++){
        //task 1
        char *test1[120];
        for(int i = 0; i < 120; i++){
            test1[i] = malloc(sizeof(char));
            free(test1[i]);
        }
        //task 2
        char *test2[120];
        for(int i = 0; i < 120; i++){
            test2[i] = malloc(sizeof(char));
        }
        for(int i = 0; i < 120; i++){
            free(test2[i]);
        }
        //task 3
        char *test3[120];
        //array to keep track of what is freed and what is not
        int allocated[120];
        for(int i = 0; i < 120;){
            //Generates a random number, either 0 or 1
            int random1 = rand() % 1;
            if(random1 == 0){
                //if 0, allocate
                test3[i] = malloc(sizeof(char));
                allocated[i] = 1;
                //increment i
                i++;
            }else{
                //else, free
                //Generates a random number between 0 and i, inclusively
                int random2 = rand() % i;
                free(test3[random2]);
                allocated[random2] = 0;
            }
        }
        //free remainig allocated objects
        for(int i = 0; i < 120; i++){
            if(allocated[i]){
                free(test3[i]);
            }
        }
        //TODO task 4 and 5
        //task 4 - Coalescing free chunks
        char *test4[120]
        for (int i = 0; i < 120; i++) {
            test4[i] = malloc(sizeof(char));
            printf("Address at index %d is %p\n", i, test4[i]);
        }
        for (int i = 0; i < 120; i++) {
            free(test4[i]);
        }
        char* test4b = malloc(100 * sizeof(char));
        printf("Starting address for longer chunk is at %p\n", test4b);
        free(test4b);
        
    }
    gettimeofday(&fin, &est);
    int time = (fin.tv_usec - start.tv_usec)/50;
    printf("Average microseconds elapsed: %d\n", time);
    return 0;
}
