/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Feb 28
 * Description: reads in long int as image data and outputs image
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img,bitShift,operationResult,zero=0;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n\n", img);
  int i,count=0;
  for (i=63;i>=0;i--){
    // use bit shift and & operator to analyze current bit
    bitShift = 0x1ul<<i;
    operationResult = bitShift&img;
    if (operationResult == zero){
      // if here the current bit is zero
      printf("  ");
    }
    else {
      // if here the current bit is one
      printf("@ ");
    }
    count = count+1;
    if (count==8){
      // print new line after 8 bits are read
      printf("\n");
      count = 0;
    }
  }
  return 0;
}
