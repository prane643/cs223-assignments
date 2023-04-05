/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Apr 5
 * Description: Compute mandelbrot set using MT
 ---------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"


// define struct to pass information into threads
struct threadInformation {
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int col1;
  int col2;
  int row1;
  int row2;
  struct ppm_pixel* imageData;
};

void *computeImage(void *arg) {
  // allocate memory for image
  
}






int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

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

  // create memory to store image
  struct ppm_pixel *image = malloc(sizeof(struct ppm_pixel)*size*size);

  // set up timer
  double timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  // create four threads
  int N=4;
  pthread_t *threads;
  threads = (pthread_t *)malloc(sizeof(pthread_t)*N);

  // run thread 1
  struct threadInformation *threadInformation;
  threadInformation = malloc(sizeof(struct threadInformation));
  (*threadInformation).xmin = xmin;
  (*threadInformation).xmax = (xmin+xmax)/2.0;
  (*threadInformation).ymin = ymin;
  (*threadInformation).ymax = (ymax+ymin)/2.0;
  (*threadInformation).col1 = 0;
  (*threadInformation).col2 = size/2;
  (*threadInformation).row1 = 0;
  (*threadInformation).row2 = size/2;
  (*threadInformation).imageData = image;
  pthread_create(&threads[0],NULL,computeImage,(void *) threadInformation);

  // run thread 2
  threadInformation = malloc(sizeof(struct threadInformation));
  (*threadInformation).xmin = (xmin+xmax)/2.0;
  (*threadInformation).xmax = xmax;
  (*threadInformation).ymin = ymin;
  (*threadInformation).ymax = (ymax+ymin)/2.0;
  (*threadInformation).col1 = size/2;
  (*threadInformation).col2 = size;
  (*threadInformation).row1 = 0;
  (*threadInformation).row2 = size/2;
  (*threadInformation).imageData = image;
  pthread_create(&threads[1],NULL,computeImage,(void *) threadInformation);
  
  // run thread 3
  threadInformation = malloc(sizeof(struct threadInformation));
  (*threadInformation).xmin = xmin;
  (*threadInformation).xmax = (xmin+xmax)/2.0;
  (*threadInformation).ymin = (ymax+ymin)/2.0;
  (*threadInformation).ymax = ymax;
  (*threadInformation).col1 = 0;
  (*threadInformation).col2 = size/2;
  (*threadInformation).row1 = size/2;
  (*threadInformation).row2 = size;
  (*threadInformation).imageData = image;
  pthread_create(&threads[2],NULL,computeImage,(void *) threadInformation);

  // run thread 4
  threadInformation = malloc(sizeof(struct threadInformation));
  (*threadInformation).xmin = (xmin+xmax)/2.0;
  (*threadInformation).xmax = xmax;
  (*threadInformation).ymin = (ymax+ymin)/2.0;
  (*threadInformation).ymax = ymax;
  (*threadInformation).col1 = size/2;
  (*threadInformation).col2 = size;
  (*threadInformation).row1 = size/2;
  (*threadInformation).row2 = size;
  (*threadInformation).imageData = image;
  pthread_create(&threads[3],NULL,computeImage,(void *) threadInformation);

  return 0;
}
