/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Feb 21
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
  // print out rgb triplets
  printf("Testing file: %s: %d %d\n","feep-raw.ppm",h,w);
  int i,j,idx;
  for (i=0;i<w;i++) {
    for (j=0;j<h;j++) {
      idx = i*h + j;
      printf("(%d,%d,%d) ",pixels[idx].red,pixels[idx].green,pixels[idx].blue);
    }
    printf("\n");
  }
  printf("\n");
  free(pixels);
  return 0;
}

