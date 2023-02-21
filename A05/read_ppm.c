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
  // skip header until image size is shown
  char buff[5000];
  fgets(buff,128,infile);
  fgets(buff,128,infile);
  fscanf(infile, " %d %d%*c",h,w);
  fgets(buff,5000,infile);
  // start reading image data
  struct ppm_pixel* p = malloc(sizeof(struct ppm_pixel)*(*w)*(*h));
  // return NULL if memory error
  if (p==NULL) {
    return NULL;
  }
  fread(p, sizeof(struct ppm_pixel),(*w)*(*h),infile);
  free(infile);
  return p;
}


struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}