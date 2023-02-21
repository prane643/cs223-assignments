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
  int w, h;
  struct ppm_pixel* pixels1 = read_ppm("feep-raw.ppm", &w, &h);
  // test writing the file to test.ppm, reload it, and print the contents
  // write data
  write_ppm("test.ppm",pixels1,w,h);
  struct ppm_pixel* pixels = read_ppm("test.ppm", &w, &h);
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
