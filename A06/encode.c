/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"


int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
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
  // read in phrase from user
  char *phrase;
  phrase = malloc(sizeof(char)*(numOfPixels*3)/8);
  printf("\nEnter phrase: ");
  scanf(" %s",phrase);
  int strLen,i,currentChar;
  strLen = strlen(phrase);
  int num,rem,bin=0b0;//,pixelIdx=8,changePixel=0;
  for(i=0;i<strLen;i++) {
    if (i>0) {
      bin = bin<<8;
    }
    currentChar = phrase[i];
    // convert current Char to 8 bit binary
    int i1 = 0,i2=0;
    num = currentChar;
    while(i2<8) {
      rem = num%2;
      num = num/2;
      if (num==currentChar/2) {
        bin = (bin + rem)<<i1;
      }
      else {
        bin = bin + (rem<<i1);
      }
      i1 = i1+1;
      i2 = i2+1;
    }
  }
  bin = bin << 8;
  // now we have the text data stored in bin as an integer value
  int pixelIdx=0,val,bitmask,shift=8*strLen,count=0;
  while(1==1) {
    bitmask = 0b1<<(shift-1);
    shift = shift-1;
    val = bin & bitmask;
    if (val==0) {
      // if here then bit was 0
      if (p[pixelIdx].red%2==1) {
        p[pixelIdx].red = p[pixelIdx].red+1;
      }
    }
    else {
      // if here then bit was 1
      if (p[pixelIdx].red%2==0) {
        p[pixelIdx].red = p[pixelIdx].red+1;
      }    
    }
    count = count +1;
    if (count==strLen*8+1) {
      break;
    }

    bitmask = 0b1<<(shift-1);
    shift = shift-1;
    val = bin & bitmask;
    if (val==0) {
      // if here then bit was 0
      if (p[pixelIdx].green%2==1) {
        p[pixelIdx].green = p[pixelIdx].green+1;
      }
    }
    else {
      // if here then bit was 1
      if (p[pixelIdx].green%2==0) {
        p[pixelIdx].green = p[pixelIdx].green+1;
      }   
    }
    count = count +1;
    if (count==strLen*8+1) {
      break;
    }

    bitmask = 0b1<<(shift-1);
    shift = shift-1;
    val = bin & bitmask;
    if (val==0) {
      // if here then bit was 0
      if (p[pixelIdx].blue%2==1) {
        p[pixelIdx].blue = p[pixelIdx].blue+1;
      }
    }
    else {
      // if here then bit was 1
      if (p[pixelIdx].blue%2==0) {
        p[pixelIdx].blue = p[pixelIdx].blue+1;
      }    
    }
    count = count +1;
    if (count==strLen*8+1) {
      break;
    }
    pixelIdx = pixelIdx+1;
  }
  write_ppm("encoded.ppm",p,width,height);
  free(p);
  return 0;
}

