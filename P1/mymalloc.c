#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"

#define MEMLENGTH 4096
static union {
    char bytes[MEMLENGTH];
    double not_used;
} heap;

struct node {
    int allocated;
    int size;
};

static int initialized;

static void leak_checker(void){
    int objects = 0;
    int bytes = 0;
    struct node *header = (struct node *) heap.bytes;
    int offset = header->size;
    int totaloffset;
    for(int i = 0; i < MEMLENGTH; i += offset){
        offset = header->size;
        if(header->allocated){
            objects++;
            bytes += header->size;
        }
        totaloffset += offset;
        header = (struct node *) (heap.bytes + totaloffset);
    }
    if(objects == 0){
        return 0;
    }else{
        printf("mymalloc: %d bytes leaked in %d objects.", bytes, objects);
    }
}

static void initialize_heap(void){
    initialized = 1;
    atexit(leak_checker);
}

void *mymalloc(size_t size, char *file, int line){
    if(size <= 0){        
        printf("malloc: Unable to allocate %zu bytes (%c:%d)", size, *file, line);
        return NULL;
    }

    if(size > MEMLENGTH){
        printf("malloc: Unable to allocate %zu bytes (%c:%d)", size, *file, line);
        return NULL;
    }

    if(!initialized) initialize_heap();

    struct node *first_header = (struct node *) heap.bytes;
    if(!(first_header->allocated)){
        first_header->allocated = 1;
        first_header->size = size;
        return first_header;
    }else{
        int offset = first_header->size;
        struct node *header = (struct node *) (heap.bytes + offset);
        while(header->allocated){
            offset += header->size;
            header = (struct node *) (heap.bytes + offset);
        }
        header->allocated = 1;
        header->size = size;
        return header;
    }

}

void myfree(void *ptr, char *file, int line){
    if(initialized == NULL){
        printf("free: Inappropriate pointer (%c:%d)", *file, line);
        exit(2);
    }

    struct node *header = ptr;
    if(!(header->allocated)){
        printf("free: Inappropriate pointer (%c:%d)", *file, line);
        exit(2);
    }else{
        
    }
}