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
  p = read_ppm("tiny_encoded.ppm",&width,&height);

  return 0;
}

