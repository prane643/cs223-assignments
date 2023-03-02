/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "read_ppm.c"

int main() {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);
  int i,j,idx;
  for (i=0;i<h;i++) {
    for (j=0;j<w;j++) {
      idx = i*w+j;
      printf("(%d,%d,%d) ",pixels[idx].red,pixels[idx].green,pixels[idx].blue);
    }
    printf("\n");
  }
  printf("\n");
  free(pixels);
  return 0;
}

