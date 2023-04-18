/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Apr 17
 * Description: New implementation of malloc+free
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int used;
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  if (size==0) {
    return NULL;
  }
  struct chunk *next = flist;
  struct chunk *prev = NULL;
  while (next!=NULL) { // while linked list is not traversed
    if (next->size>=size) { // if head of linked list has enough space
      if (prev!=NULL) { 
        prev->next = next->next; // remove wante chunk and patch the linked list together
      }
      else { // if we were at the head of the list
        flist = next->next; // update free list to start at next chunk
      }
      return (void*)(next+1); // return pointer to the chunk of right size (skip over header)
    }
    else { // if next chunk doesn't have enough space, iterate through linked list
      prev = next;
      next = next->next;
    }
  }
}

void free(void *memory) {
  if (memory!=NULL) {
    struct chunk *cnk = (struct chunk*)((struct chunk*)memory-1);
    cnk->next = flist; 
    flist = cnk; // adds "memory" to head of free list
  }
  return;
}
