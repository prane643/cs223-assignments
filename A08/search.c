/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Mar 27
 * Description: Using two processes to search for a value in data.bin
 ---------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>

int main() {
  //struct point p[3];
  FILE* fp = fopen("data.bin", "rb");
  int *numOfInt = malloc(sizeof(int));
  fread(numOfInt, sizeof(int),1, fp);
  //printf("Number of ints: %d",*numOfInt);
  int *intList = malloc(sizeof(int)*(*numOfInt));
  fread(intList, sizeof(int),*numOfInt, fp);
  // look for integer

  fclose(fp);
  free(numOfInt);
  return 0;
}
