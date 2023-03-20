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
    return 0;
  }
  int width,height;
  struct ppm_pixel *p;
  p = read_ppm(argv[1],&width,&height);
  // p is a pointer to the pixel information of the image
  if (p==NULL) {
    printf("\n Error: file cannot be read\n");
    exit(1);
  }
  printf("Reading %s with width %d and height %d \n",argv[1],width,height);
  int numOfPixels;
  numOfPixels = width*height;
  printf("Max number of characters in the image: %d\n",(numOfPixels*3)/8);
  // find secret message
  int i,redVal,greenVal,blueVal,count=0;
  int message = 0b0;
  printf("\n");
  for (i=0;i<numOfPixels;i++) {
      redVal = p[i].red;
      greenVal = p[i].green;
      blueVal = p[i].blue;
      // now check if even or odd, use bitshift to store value
      if (count==0) {
        message = message + redVal%2;
      }
      else {
        message = (message<<1) + redVal%2;
      }
      count = count+1;
      if (count==8) {
        // if count is 8, then message contains character
        if (message==0) {
          free(p);
          return 0;
        }
        printf("%c",message);
        count = 0;
        message = 0b0;
      }
      // now check if even or odd, use bitshift to store value
      if (count==0) {
        message = message + greenVal%2;
      }
      else {
        message = (message<<1) + greenVal%2;
      }
      count = count+1;
      if (count==8) {
        // if count is 8, then message contains character
        if (message==0) {
          free(p);
          return 0;
        }
        printf("%c",message);
        count = 0;
        message = 0b0;
      }
      // now check if even or odd, use bitshift to store value
      if (count==0) {
        message = message + blueVal%2;
      }
      else {
        message = (message<<1) + blueVal%2;
      }
      count = count+1;
      if (count==8) {
        // if count is 8, then message contains character
        if (message==0) {
          free(p);
          return 0;
        }
        printf("%c",message);
        count = 0;
        message = 0b0;
      }
  }
  printf("\n");
  free(p);
  return 0;
}
