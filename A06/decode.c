/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Feb 28
 * Description: decode hidden messages from .ppm
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    //return 0;
  }
  int width,height;
  struct ppm_pixel *p;
  p = read_ppm("tiny_encoded.ppm",&width,&height);
  // p is a pointer to the pixel information of the image
  printf("\n Width: %d, Height: %d \n",width,height);
  int numOfPixels;
  numOfPixels = width*height;
  printf("Max number of characters in the image: %d\n",(numOfPixels*3)/8);
  // find secret message
  int i,redVal,greenVal,blueVal,count=0;
  unsigned long message = 0b0;
  //message = malloc(sizeof(int)*numOfPixels*3);
  for (i=0;i<numOfPixels;i++) {
      redVal = p[i].red;
      greenVal = p[i].green;
      blueVal = p[i].blue;
      // now check if even or odd
      if (i==0) {
        message = message + redVal%2;
      }
      else {
        message = (message<<1) + redVal%2;
      }
      count = count+1;
      message = (message<<1) + greenVal%2;
      count = count+1;
      message = (message<<1) + blueVal%2;
      count = count+1;
  }
  count = count-1;
  //int c = 0b00000001<<4;
  char c = strtol(message, 0, 2);
  printf("\n%02lX\n",message);
  /*
  for (i=0;i<count;i++){
    printf("%d",message[i]);
  }
  */
  return 0;
}

