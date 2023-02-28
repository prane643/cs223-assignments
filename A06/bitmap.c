/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Feb 28
 * Description: reads in long int as image data and outputs image
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n\n", img);
  unsigned long test;
  int i;
  for (i=63;i>=0;i--){
  test = 1<<i;
  printf("%lx \n",test);
  if (test&img == 0){
    // if here the current bit is zero
    printf(" ");
  }
  else {
    // if here the current bit is one
    printf("@");
  }
  }
  // todo: your code here
  return 0;
}
