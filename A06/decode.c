/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Feb 28
 * Description: decode hidden messages from .ppm
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"



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
    printf("usage: decode <file.ppm>\n");
    //return 0;
  }
  int width,height;
  struct ppm_pixel *p;
  p = read_ppm("monalisa-encoded.ppm",&width,&height);
  // p is a pointer to the pixel information of the image
  printf("\n Width: %d, Height: %d \n",width,height);
  int numOfPixels;
  numOfPixels = width*height;
  printf("Max number of characters in the image: %d\n",(numOfPixels*3)/8);
  // find secret message
  int i,redVal,greenVal,blueVal,count=0;
  int message = 0b0;
  printf("\n");
  //message = malloc(sizeof(int)*numOfPixels*3);
  for (i=0;i<numOfPixels;i++) {
      redVal = p[i].red;
      greenVal = p[i].green;
      blueVal = p[i].blue;
      // now check if even or odd
      if (count==0) {
        message = message + redVal%2;
      }
      else {
        message = (message<<1) + redVal%2;
      }
      count = count+1;
      if (count==8) {
        if (message==0) {
          return 0;
        }
        printf("%c",message);
        count = 0;
        message = 0b0;
      }
      if (count==0) {
        message = message + greenVal%2;
      }
      else {
        message = (message<<1) + greenVal%2;
      }
      //message = (message<<1) + greenVal%2;
      count = count+1;
      if (count==8) {
        if (message==0) {
          return 0;
        }
        printf("%c",message);
        count = 0;
        message = 0b0;
      }
      if (count==0) {
        message = message + blueVal%2;
      }
      else {
        message = (message<<1) + blueVal%2;
      }
      //message = (message<<1) + blueVal%2;
      count = count+1;
      if (count==8) {
        if (message==0) {
          return 0;
        }
        printf("%c",message);
        count = 0;
        message = 0b0;
      }
  }
  printf("\n");
  //int c = 0b0110000101100001;
  //char c = strtol(message, 0, 2);
  //printf("\n%c\n",c);
  /*
  for (i=0;i<count;i++){
    printf("%d",message[i]);
  }
  */
  return 0;
}
