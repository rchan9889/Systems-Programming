#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
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
        //free remaining allocated objects
        for(int i = 0; i < 120; i++){
            if(allocated[i]){
                free(test3[i]);
            }
        }
        
        //Task 4
        char *test4[120]
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
        //Uses malloc and free to find all prime numbers below 100
        int *test5 = malloc(100 * sizeof(int));
        test5[0] = 0;
        test5[1] = 0;
        test5[2] = 1;
        for (int i = 3; i < 100; i++) {
            test5[i] = 1;
            for (int j = 2; j <= (int) sqrt((double); j++) {
                if (i % j == 0) {
                    test5[i] = 0;
                }
            }
        }
        printf("Prime numbers below 100:\n");
        for (int i = 2; i < 100; i++) {
            if (test5[i] == 1) {
                printf("%d ", i);
            }    
        }
        
        print("\n");
        free(test5);
        
        
        
    }
    gettimeofday(&fin, &est);
    int time = (fin.tv_usec - start.tv_usec)/50;
    printf("Average microseconds elapsed: %d\n", time);
    return 0;
}
