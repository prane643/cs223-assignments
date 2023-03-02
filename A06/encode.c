/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "read_ppm.h"
#include "write_ppm.h"

struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) {
  FILE *infile;
  infile = fopen(filename,"rb");
  if (infile==NULL) {
    return NULL;
  }
  // skip through header
  char buff[128],c1;
  fgets(buff,128,infile); // P6
  c1 = getc(infile);
  if (c1=='#') {
    // if here there is a comment, so skip
    ungetc(c1,infile);
    fgets(buff,128,infile);
  }
  else {
    ungetc(c1,infile);
  }
  fscanf(infile," %d %d%*c",h,w); // size
  fgets(buff,128,infile);
  struct ppm_pixel* p = malloc(sizeof(struct ppm_pixel)*(*w)*(*h));
  if (p==NULL) {
    return NULL;
  }
  fread(p,sizeof(struct ppm_pixel),(*w)*(*h),infile);
  fclose(infile);
  return p;
}




int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    //return 0;
  }
  int width,height;
  struct ppm_pixel *p;
  //p = read_ppm(argv[1],&width,&height);
  p = read_ppm("feep-raw.ppm",&width,&height);
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
  int num,rem,bin=0b0,pixelIdx=0;
  for(i=0;i<strLen;i++) {
    currentChar = phrase[i];
    // convert current Char to 8 bit binary
    num = currentChar;
    while(num>0) {
      rem = num%2;
      num = num/2;
      if (num==currentChar/2) {
        bin = bin + rem;
      }
      else {
        bin = (bin<<1)+rem;
      }
      // now change pixel data depending if rem is 0 or 1
      if (rem==1) {

      }
    }
    if (bin!=currentChar) {
      printf("\n Error, bin!=currentChar");
      exit(1);
    }
  int ch; //= 0b0;
  ch = phrase[0];
  }

  free(p);
  return 0;
  
}

