/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Mar 22
 * Description: Computing image for mandelbrot set
 ---------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // allocate memory for image
  struct ppm_pixel *image = malloc(sizeof(struct ppm_pixel)*size*size);

  // generate pallet
  srand(time(0));
  unsigned char *pallete;
  int idx,i1,j1;
  pallete = malloc(sizeof(unsigned char)*maxIterations*3);
  for (i1=0;i1<maxIterations;i1++) {
    for (j1=0;j1<3;j1++) {
      idx = i1*3+j1;
      pallete[idx]=rand()%255;
    }
  }
  
  // run algorithm
  double timer;
  struct timeval tstart, tend;
  int iter,imgIdx=0;
  unsigned char r,g,b;
  float i,j;
  float xfrac,yfrac,x0,y0,x,y,xtmp;
  struct ppm_pixel pixel;
  // start timer
  gettimeofday(&tstart, NULL);
  for (i=0;i<size;i++) {
    for (j=0;j<size;j++) {
      xfrac = j/size;
      yfrac = i/size;
      x0 = xmin+xfrac*(xmax-xmin);
      y0 = ymin+yfrac*(ymax-ymin);
      x = 0;
      y = 0;
      iter = 0;
      while (iter<maxIterations && (x*x+y*y)<2*2) {
        xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter = iter+1;
      }
      if (iter < maxIterations) {
        r = pallete[iter*3];
        g = pallete[iter*3+1];
        b = pallete[iter*3+2];
        pixel.red = r;
        pixel.green = g;
        pixel.blue = b;
      }
      else {
        pixel.red = 0;
        pixel.green = 0;
        pixel.blue = 0;
      }
      image[imgIdx] = pixel;
      imgIdx++;
    }
  }
  gettimeofday(&tend, NULL);

  // calculate computation time
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Computed mandelbrot set (%dx%d) in %g seconds\n",size,size,timer);

  // write image
  char buffer[100];
  sprintf(buffer,"mandelbrot-%d-%ld.ppm",size,time(0));
  write_ppm(buffer,image,size,size);

  // free memory
  free(pallete);
  free(image);

  return 0;
}
