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
    for(int k = 0; k < 50; k++){
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
            int random1 = rand() % 2;
            if(random1 == 0){
                //if 0, allocate
                test3[i] = malloc(sizeof(char));
                allocated[i] = 1;
                //increment i
                i++;
            }else{
                //else, free
                //Generates a random number between 0 and i, inclusively
                int random2 = rand() % (i + 1);
                free(test3[random2]);
                allocated[random2] = 0;
            }
        }
        //free remaining allocated objects
        for(int i = 0; i < 120; i++){
            if(allocated[i]){
                free(test3[i]);
            }
        }
        
        //Task 4
        char *test4[120];
        //allocate 60 objects
        for (int i = 0; i < 60; i++) {
            test4[i] = malloc(sizeof(char));
        }
        //free 30 of the 60 objecets
        for (int i = 30; i < 60; i++) {
            free(test4[i]);
        }
        //allocate another 60 objects
        for (int i = 30; i < 90; i++){
            test4[i] = malloc(sizeof(char));
        }
        //free everything
        for (int i = 0; i < 90; i++){
            free(test4[i]);
        }

        //Task 5
        //Tests malloc with different types
        int *test5 = malloc(100 * sizeof(int));


        
        
        
        free(test5);
        
        printf("Test: %d\n", k);
        
    }
    gettimeofday(&fin, &est);
    int time = (fin.tv_usec - start.tv_usec)/50;
    printf("Average microseconds elapsed: %d\n", time);
    return 0;
}
