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
    //Testing individual errors
    //Test 1 - Size <= 0
  
    printf("Test 1a:\n");
    char* test1a = malloc(0);
    if (test1a != NULL) {
        printf("Error: memory was allocated when should have returned NULL);
        free(test1a);
    }
    printf("Test 1b:\n");
    char* test1b = malloc(-37);
    if (test1b != NULL) {
        printf("Error: memory was allocated when should have returned NULL);
        free(test1b);
    }
  
    //Test 2 - Size greater than size of heap (4096 bytes)
    printf("Test 2a:\n");
    char* test2a = malloc(4097);
    if (test2a != NULL) {
        printf("Error: memory was allocated when should have returned NULL);
        free(test2a);
    }
    printf("Test 2b:\n");
    char* test2b = malloc(880092);
    if (test2b != NULL) {
        printf("Error: memory was allocated when should have returned NULL);
        free(test2b);
    }

    //Test 3 - Size greater than remaining heap size
    printf("Test 3: first malloc 3000 bytes, then try malloc another 3000\n");
    char* test3a = malloc(3000 * sizeof(char));
    char* test3b = malloc(3000 * sizeof(char));
    if (test3a != NULL) {
        free(test3a);
    }
    if (test3b != NULL) {
        printf("Error: memory was allocated when should have returned NULL\n");
        free(test3b);
    }

    //Test 4 - Heap not initialized
    char* test4;
    free(test4);

    //Test 5 - Freeing an address not obtained from malloc()
    char* test5a = malloc(sizeof(char));
    char* test5b
    free(test5b);
    if (test5a != NULL) {
        free(test5a);
    }

    //Test 6 - Freeing address not at start of chunk
    char* test6 = malloc(sizeof(char));
    free(test6 + 1);
    if (test6 != NULL) {
        free(test6);
    }

    //Test 7 - Double freeing
    char* test7 = malloc(sizeof(char));
    free(test7);
    free(test7);
}
