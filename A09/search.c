#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct information {
  int value;
  int threadNumber;
  int *data;
  int segmentSize;
}

void *searchForValue(struct information *info) {
  // search for value in data.bin in the corresponding indices

}



int main(int argc, char** argv) {
  if (argc < 2) {
    printf("usage: %s <NumThreads>\n", argv[0]);
    exit(0);
  }
  int N;
  N = atoi(argv[0]);
  FILE* fp = fopen("data.bin", "rb");
  int *numOfInt = malloc(sizeof(int));
  fread(numOfInt, sizeof(int),1, fp);
  printf("\nNumber of ints: %d",*numOfInt);
  int *intList = malloc(sizeof(int)*(*numOfInt));
  fread(intList, sizeof(int),*numOfInt, fp);
  // split data into N parts
  int segmentSize;
  segmentSize = (*numOfInt)/N;
  int remaining;
  remaining = (*numOfInt)%N;
  int i,j,count=0;
  //int *segmentList = malloc(sizeof(int)*segmentSize*(N-1));
  //int *lastSegmentList = malloc(sizeof(int)*segmentSize+remaining);
  // get input to look for 
  printf("\nEnter a value to search: ");
  int input;
  scanf(" %d",&input);
  // define threads
  pthread_t threads[input];
  struct information threadInformation[N];
  for (i=0;i<N;i++) {
    threadInformation[i].value = input;
    threadInformation[i].threadNumber = i+1;
    threadInformation[i].data = intList;
    if (i!=N-1) {
      threadInformation[i].segmentSize = segmentSize;
      pthread_create(&threads[i],NULL,searchForValue,&threadInformation[i]);
    }
    else {
      threadInformation[i].segmentSize = segmentSize+remaining;
      pthread_create(&threads[i],NULL,searchForValue,&threadInformation[i]);
    }
  }
  return 0;
  }




