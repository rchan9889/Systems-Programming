#include <stdint.h>
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

static int leak_checker(void){
    int objects = 0;
    int bytes = 0;
    struct node *header = (struct node *) heap.bytes;
    int offset = 0;
    /* for(int i = 0; i < MEMLENGTH; i += offset){
        offset = header->size;
        if(header->allocated){
            objects++;
            bytes += header->size;
        }else{
            offset = 8;
        }
        header = (struct node *) (heap.bytes + i);
    } */
   while(offset < MEMLENGTH){
        if(header->allocated){
            objects++;
            bytes += header->size;
        }
        offset += header->size + 8;
        header = (struct node *) (heap.bytes + offset);
   }
    if(objects == 0){
        return 0;
    }else{
        printf("mymalloc: %d bytes leaked in %d objects.", bytes, objects);
        return 1;
    }
}

static void initialize_heap(void){
    initialized = 1;

    struct node *first_header = (struct node *) heap.bytes;
    first_header->size = MEMLENGTH - 8;

    atexit(leak_checker);
}

void *mymalloc(size_t size, char *file, int line){
    int offset = 0;
    size = (size + 7) & ~7;
    
    if(size <= 0){        
        printf("malloc: Unable to allocate %zu bytes (%c:%d)", size, *file, line);
        return NULL;
    }

    if(size > MEMLENGTH){
        printf("malloc: Unable to allocate %zu bytes (%c:%d)", size, *file, line);
        return NULL;
    }

    struct node *curr_header;
    struct node *new_header;
    struct node *next_header;

    if(!initialized) initialize_heap();

    while (offset + 8 < MEMLENGTH) {
        curr_header = (struct node *) (heap.bytes + offset);

        // When current chunk isn't allocated and also big enough, we return first address in chunk 
        // and make new header if we can
        if (!(curr_header->allocated) && curr_header->size >= size)  { 
            if ((offset + 8 + curr_header->size < MEMLENGTH) && curr_header->size > size) {
                new_header = (struct node *) (heap.bytes + offset + 8 + size);
                new_header->size = curr_header->size - size;
            }
            return (struct node *) (heap.bytes + offset + 8);
        }

        // When current chunk isn't allocated but not big enough, we first check to see if we can coalesce
        // with next chunk if it is also free
        else if (!(curr_header->allocated)) {

            // Check if current chunk is last chunk. If it is, not enough space in heap
            if (offset + curr_header->size + 8 >= MEMLENGTH) {
                printf("malloc: Unable to allocate %zu bytes (%c:%d)", size, *file, line);
                return NULL;
            }
            else {
                next_header = (struct node *) (heap.bytes + offset + curr_header->size + 8);

                // Coalesces with next chunk if it is also not allocated
                if (!(next_header->allocated)) {
                    curr_header->size += (8 + next_header->size);
                    // TODO: Code that deletes the next header
                }
            }

        }
        // This if statememt shows up twice, could probably clean up later
        else if (offset + curr_header->size + 8 >= MEMLENGTH) {
            printf("malloc: Unable to allocate %zu bytes (%c:%d)", size, *file, line);
            return NULL;
        }
        else {
            offset += (curr_header->size + 8);
        }
        // If we can't coalesce chunks or current chunk is already allocated, we move on to the next header
        
    }
    // If it ever goes out of while loop, we know we don't have enough space anywhere
    printf("malloc: Unable to allocate %zu bytes (%c:%d)", size, *file, line);
    return NULL;

    /*
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
    */

}

void myfree(void *ptr, char *file, int line){
    if(initialized == 0){
        printf("free: Inappropriate pointer (%c:%d)", *file, line);
        exit(2);
    }

    // Satisfies error type #1
    if (ptr < heap.bytes + 8 || ptr > heap.bytes + MEMLENGTH) {
        printf("free: Inappropriate pointer (%c:%d)", *file, line);
        exit(2);
    }

    // Satisfies error type #2(?)
    if (*(ptr - 8) == NULL || (intptr_t) ptr % 8 != 0) {
        printf("free: Inappropriate pointer (%c:%d)", *file, line);
        exit(2);
    }

    struct node *header = (struct node *) (ptr - 8);
    
    // Satisfies error type #3
    if(!(header->allocated)){
        printf("free: Inappropriate pointer (%c:%d)", *file, line);
        exit(2);
    }else{
        header->allocated = 0;
    }
}