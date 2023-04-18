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
        prev->next = next->next;
      }
      else { // if we were at the head of the list
        flist = next->next; // update linked list to start at next chunk
      }
      return (void*)(next+1); // return pointer to the next chunk of current chunk
    }
    else { // if next chunk doesn't have enough space, iterate through linked list
      prev = next;
      next = next->next;
    }
  }
}

void free(void *memory) {
  // TODO: Implement malloc with a free list (See reading for details)
  return;
}
