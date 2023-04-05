#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct information {
  int value;
  int threadNumber;
  int *data;
  int segmentSize;
  int lastSegmentSize;
};

//void *searchForValue(struct information *tInfo) {
void *searchForValue(void *arg) { 
  struct information tInfo;
  tInfo = *((struct information)arg);
  // search for value in data.bin
  int targetValue,tNumber,size,j;
  targetValue = tInfo->value;
  tNumber = tInfo->threadNumber;
  size = tInfo->segmentSize;
  int *data;
  data = tInfo->data;
  if (tInfo->lastSegmentSize==0) {
    for (j=0;j<size;j++) {
      if (data[tNumber*size+j]==targetValue){
        //free(data);
        printf("\nThread %d found %d at index %d",tNumber+1,targetValue,tNumber*size+j);
        return 0;
      }
    }
    //free(data);
    return 0;
  }
  else {
    // if here on last segment with different number of elements
    int lastSegSize;
    lastSegSize = tInfo->lastSegmentSize;
    for (j=0;j<lastSegSize;j++) {
      if (data[tNumber*size+j]==targetValue){
        //free(data);
        printf("\nThread %d found %d at index %d",tNumber+1,targetValue,tNumber*size+j);
        return 0;
      }
    }
    //free(data);
    return 0;
  }
  //free(data);
  return 0;
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
  int i;//j,count=0;
  //int *segmentList = malloc(sizeof(int)*segmentSize*(N-1));
  //int *lastSegmentList = malloc(sizeof(int)*segmentSize+remaining);
  // get input to look for 
  printf("\nEnter a value to search: ");
  int input;
  scanf(" %d",&input);
  // define threads
  //pthread_t threads[N];
  pthread_t *threads;
  threads = (pthread_t *)malloc(sizeof(pthread_t)*N);
  struct information threadInformation[N];
  for (i=0;i<N;i++) {
    threadInformation[i].value = input;
    threadInformation[i].threadNumber = i;
    threadInformation[i].data = intList;
    if (i!=N-1) {
      threadInformation[i].segmentSize = segmentSize;
      threadInformation[i].lastSegmentSize = 0;
      pthread_create(&threads[i],NULL,searchForValue,&threadInformation[i]);
    }
    else {
      threadInformation[i].segmentSize = segmentSize;
      threadInformation[i].lastSegmentSize = segmentSize+remaining;
      pthread_create(&threads[i],NULL,searchForValue,&threadInformation[i]);
    }
  }
  // join threads together
  for (i=0;i<N;i++) {
    pthread_join(threads[i],NULL);
  }
  fclose(fp);
  free(numOfInt);
  free(intList);
  return 0;
  }




