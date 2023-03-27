/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Mar 27
 * Description: Using two processes to search for a value in data.bin
 ---------------------------------------------*/


#include <stdio.h>
# include <stdlib.h>

int main() {
  //struct point p[3];
  FILE* fp = fopen("data.bin", "rb");
  int *numOfInt;
  fread(numOfInt, sizeof(int),1, fp);
  



  fclose(fp);
  return 0;
}
