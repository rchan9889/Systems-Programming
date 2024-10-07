#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mymalloc.c"

#ifndef REALMALLOC
#include "mymalloc.h"
#endif

int main(int argc, char **argv){
    //Testing coalescing chunks in memory
    char *test1a[120]
    for (int i = 0; i < 120; i++) {
        test1a[i] = malloc(sizeof(char));
    }
    printf("Starting address for first chunk is at %p\n", test1a[0]);
        
    for (int i = 0; i < 120; i++) {
        free(test1a[i]);
    }

    //If starting address for larger array is still at start of heap, the multiple previous freed chunks have successfully coalesced
    char* test1b = malloc(100 * sizeof(char));
    printf("Starting address for longer chunk is at %p\n", test1b);
    free(test1b);


}
