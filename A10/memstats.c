/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Apr 18
 * Description: Output memory stats for new malloc+free
 ---------------------------------------------*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk {
  int size;
  int used;
  struct chunk *next;
};

void memstats(struct chunk* freelist, void* buffer[], int len) {
  // calculate # of used blocks and the bytes
  int i,numOfUsed=0,numOfFree=0,numOfTotal;
  int bytesFree=0,bytesUsed=0,bytesTotal=0,openMem=0;
  struct chunk* headerInfo;
  for (i=0;i<len;i++) {
    if (buffer[i]!=NULL) {
      headerInfo = (struct chunk*)((struct chunk*)buffer[i]-1);
      bytesUsed = bytesUsed + headerInfo->size;
      openMem = openMem + (headerInfo->size - headerInfo->used);
      numOfUsed++;
    }
  }
  // calculate free blocks
  struct chunk* nextNode = freelist;
  if (nextNode!=NULL) {
    while (1) {
      bytesFree = bytesFree + nextNode->size;
      numOfFree++;
      nextNode = nextNode->next;
      if (nextNode==NULL) {
        break;
      }
    }
  }
  else {
    numOfFree = 0;
  }
  // calculate total blocks
  numOfTotal = numOfFree+numOfUsed;
  printf("Total blocks: %d Free blocks: %d"
    " Used blocks: %d",numOfTotal,numOfFree,numOfUsed);
  // calculate total bytes
  bytesTotal = bytesFree+bytesUsed;
  printf("\nTotal memory allocated: %d Free memory: %d"
    " Used memory: %d",bytesTotal,bytesFree,bytesUsed);
  // print percentage of underutilized memory
  float undUtilMem;
  undUtilMem =  openMem/(float)bytesUsed;
  printf("\nUnderutilized memory: %.2f\n",undUtilMem);
}

int main ( int argc, char* argv[]) {
  printf("Starting test..\n");
  srand(100);
  double timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }
  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0 ; j < ROUNDS; j++) {
    printf("---------------\n%d\n" , j);
    for (int i= 0 ; i < LOOP ; i++) {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL) {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      } 
      else {
        size_t size = (size_t) randExp(8, 4000); 
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return(1);
        } 
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int) size, index);
      }
    }
    extern struct chunk* flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;
    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]); 
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Time is %g\n", timer);
  return 0 ;
}