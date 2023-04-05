#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("usage: %s <NumThreads>\n", argv[0]);
    exit(0);
  }
  FILE* fp = fopen("data.bin", "rb");
  int *numOfInt = malloc(sizeof(int));
  fread(numOfInt, sizeof(int),1, fp);
  printf("\nNumber of ints: %d",*numOfInt);
  int *intList = malloc(sizeof(int)*(*numOfInt));
  fread(intList, sizeof(int),*numOfInt, fp);
  // look for integer
  printf("\nEnter a value to search: ");
  int input;
  scanf(" %d",&input);
  pthread_t threads[input];
  int i,idx1=0,idx2=(*numOfInt)/atoi(argv[0]);
  for (i=0;i<atoi(argv[0]);i++) {
    pthread_create(&threads[i],NULL,searchForValue,input,idx1,idx2,i+1,intList);
  }

  return 0;
}

struct information {
  int value;
  int idx1;
  int idx2;
  int threadNumber;
  int *data;
}

void *searchForValue(struct information *info) {
  // search for value in data.bin in the corresponding indices

}