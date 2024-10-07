#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mymalloc.c"

#ifndef REALMALLOC
#include "mymalloc.h"
#endif

int main(int argc, char **argv){
    int start = gettimeofday();
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
        //tast 4
    }
    int fin = gettimeofday();
    int time = (fin - start)/50;
    printf("%d", time);
    return 0;
}