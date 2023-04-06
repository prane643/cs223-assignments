/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Feb 21
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

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

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}
