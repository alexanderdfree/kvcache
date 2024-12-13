
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "kv_store.h"

/**
 * Purpose: deserialize a key-value pair buffer (the format is in the README
 * file).
 * 
 * Inputs:
 * - `char *buf`: pointer to the key-value pair buffer.
 * - `size_t buffer_size`: total size of the key-value pair buffer (in bytes).
 * 
 * Return: `KVPAIR *start_of_list`, which is a pointer to start of linked list
 * of `KVPAIR`s if successful. Otherwise, return NULL.
 * 
 * Assumptions: `buf` is not empty or NULL and the `buffer_size` is the correct
 * size of the buffer.
 *
 * Hints: 
 * - Although you could probably make this function recursive, our
 *   implementation just uses a `while` loop.
 * - You will probably have a pointer which will go through the buffer. You
 *   first should probably think about how long you should be iterating for.
 * - Once you have figured out the correct bounds for iteration, for each new
 *   key-value pair you see, you will probably want to allocate a new node in
 *   your list, and then populate all the fields in that node. This will
 *   probably involve calling `memcpy()` a couple of times, and also `malloc()`
 *   for the value.
 * - Once you're done populating an individual node of the linked list, you are
 *   to actually add it to the list. This can be done by appending the node to
 *   the tail of the list. Think about how to do this...
 */

KVPAIR *deserialize(char *buf, size_t buffer_size) {
    //create a pointer to a node, will return this later
    //KVPAIR a;
    //printf("beginning");
    KVPAIR *head = NULL;
    KVPAIR *tail = NULL;

    
    char* bufPointer = buf;
    int i = 0;
    //int KVTracker = 1;
    //printf("before loop");
    while(i < buffer_size){
        
        //key
        long k;
        memcpy((void*)&k, bufPointer, sizeof(long));
        bufPointer += sizeof(long);
        
        //size
        int valueSize;
        memcpy((void*)&valueSize, bufPointer, sizeof(int));
        bufPointer += sizeof(int);

        //value
        char* value = malloc(sizeof(char) * valueSize); //+1
        memcpy((char*)value, bufPointer, (size_t)valueSize);
        //value[valueSize] = 0;
        bufPointer += valueSize;
    

        //add to kv
        //printf("before kv");
        KVPAIR *kv = (KVPAIR*)malloc(sizeof(KVPAIR));
        kv->next = NULL;
        kv->val = value;
        //printf("value done");
        kv->key = k;
        kv->size = valueSize;
        //printf("key size done");
        if (tail == NULL){
            //printf("tail = kv");
            head = kv;
            tail = kv;
            
        }
        else{
            tail->next = kv;
            tail = kv;
            //printf("tail next");
        }
        
        

        //move forward
        //tail = kv;
        
        //printlist(head);
        i+=sizeof(long)+sizeof(int)+valueSize*sizeof(char);
    }
    //(*kvPointer).next = NULL;
    //printlist(head);
    return head;
    //return NULL;
}







/**
 * Purpose: Look up the provided key in a `KVPAIR` list.
 * 
 * Inputs: 
 * - `KVPAIR *list`: pointer to the start of the linked list of `KVPAIR`s.
 * - `long key`: the key to look up in the linked list.
 * 
 * Return: `KVPAIR *found_entry`, which is pointer to the entry that contains
 * the key or NULL, if the key is not found.
 */

KVPAIR *lookup(KVPAIR *list, long key) {
    
    while(list != NULL){
        if ((*list).key == key) return list;
        list = (*list).next;
    }
    return NULL;
}


/**
 * Purpose: Delete the `KVPAIR` with the specified key from the linked list.
 * 
 * Inputs: 
 * - `KVPAIR **list`: pointer to the pointer (handle) of first key-value pair.
 * - `long key`: the key to the `KVPAIR` entry that should be deleted.
 * 
 * Return: 1 if the key was found in the list and deleted successfully. 0 if the
 * key was not found in the list and/or some error occured during the deletion.
 * 
 * Hints:
 * - You will need to change `KVPAIR`'s next pointers in the list and free
 *   memory.
 * - Think about what needs to be updated if the first entry in the list is
 *   deleted...
 */

int delete(KVPAIR **list, long key) {
   
    if (*list == NULL) return 0;
    KVPAIR *previous = *list;
    if ((*previous).key == key){
            *list = (**list).next;
            /*free(&(*previous).size);
            free(&(*previous).key);
            free(&(*previous).val);*/
            //free(&(*previous).next);
            free(previous->val);
            free(previous);
            
            return 1;
    }
    KVPAIR *current = (*previous).next;
    
    while(current != NULL){
        if ((*current).key == key){
            (*previous).next = (*current).next;
            /*free(&((*current).size)); 
            free(&(*current).key);
            free(&(*current).val);*/
            //free(&(*current).next);
            free(current->val);
            free(current);
            
            return 1;
        }
        previous = current;
        current = (*current).next;
    }

    return 0;
}
