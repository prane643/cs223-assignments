/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Mar 27
 * Description: Using two processes to search for a value in data.bin
 ---------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  //struct point p[3];
  FILE* fp = fopen("data.bin", "rb");
  int *numOfInt = malloc(sizeof(int));
  fread(numOfInt, sizeof(int),1, fp);
  //printf("Number of ints: %d",*numOfInt);
  int *intList = malloc(sizeof(int)*(*numOfInt));
  fread(intList, sizeof(int),*numOfInt, fp);
  // look for integer
  printf("\nEnter a value to search: ");
  int input;
  scanf(" %d",&input);
  // start looking for integer
  pid_t pid;
  int split = (*numOfInt)/2;
  int i,j;
  pid = fork();
  if (pid!=0) {
    // parent process
    for(i=0;i<split;i++) {
      if (intList[i]==input) {
        printf("Parent found %d at index %d\n",input,i);
        fclose(fp);
        free(numOfInt);
        free(intList);
        return 0;
      }
    }
    // if here value was not found
    printf("Not found!\n");
  }
  else {
    // child process
    for (j=split;j<(*numOfInt);j++) {
      if (intList[j]==input) {
        printf("Child found %d at index %d\n",input,j);
        fclose(fp);
        free(numOfInt);
        free(intList);
        return 0;
      }   
    }
    // if here value was not found
    printf("Not found!\n");
  }

  fclose(fp);
  free(numOfInt);
  free(intList);
  return 0;
}
