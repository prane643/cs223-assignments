/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Feb 19
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // read two words from user
  printf("\n Enter two words separated by one comma: ");
  char inputStr[128];
  char w1[128];
  char w2[128];
  scanf(" %s",inputStr);
  char *token;
  const char delim[4]=",";
  token = strtok(inputStr,delim);
  strcpy(w1,token);
  token = strtok(NULL,delim);
  strcpy(w2,token);
  // find idx of common letter between w1 and w2
  int w1Len,w2Len,i,j;
  int breakFlag = 0;
  w1Len = strlen(w1);
  w2Len = strlen(w2);
  for (i=0;i<w1Len;i++) {
    for (j=0;j<w2Len;j++) {
      if (w1[i]==w2[j]) {
        breakFlag = 1;
        break;
      }
    }
    if (breakFlag==1) {
      break;
    }
  }
  // if no common letters, tell user
  if (breakFlag==0){
    printf("\nThere are no common letters \n\n");
    exit(1);
  }
  // else, create 2d array in heap
  int *two_d_array;
  two_d_array = malloc(sizeof(int)*(w1Len*w2Len));
  // insert first work verticlaly in correct spot
  int h,k,idx,index2=0,index1=0;
  for (h=0;h<w1Len;h++) {
    for (k=0;k<w2Len;k++) {
      // convert to linear index
      idx = h*w2Len+k;
      if (h==i ) {
        two_d_array[idx] = w2[index2];
        index2 = index2+1;
      }
      if (k==j) {
        two_d_array[idx] = w1[index1];
        index1 = index1+1;
      }
      if (h!=i && k!=j) {
        two_d_array[idx] = '.';
      }
    }
  }
  // print out 2d array
  for (h=0;h<w1Len;h++) {
    printf("\n");
    for (k=0;k<w2Len;k++) {
      idx = h*w2Len+k;
      printf("%c ",two_d_array[idx]);
    }
  }
  printf("\n\n");
  // free memory
  free(two_d_array);
  return 0;
}
