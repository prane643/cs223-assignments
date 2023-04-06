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
#include <sys/time.h>
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
  int size;
  int maxIterations;
  unsigned char *pallete;
  struct ppm_pixel* imageData;
};

void *computeImage(void *arg) {
  struct threadInformation *tInfo;
  tInfo = (struct threadInformation*)arg;
  // define some variables
  int col1,col2,row1,row2,size;
  int maxIterations,istart,ifinish,jstart,jfinish;
  float xmin,xmax,ymin,ymax;
  struct ppm_pixel* image;
  unsigned char *pallete;
  // identify thread
  col1 = tInfo->col1;
  col2 = tInfo->col2;
  row1 = tInfo->row1;
  row2 = tInfo->row2;
  printf("Thread %d) Sub-image block: cols (%d,%d) to rows (%d,%d)\n",
    pthread_self(),col1,col2,row1,row2);
  size = tInfo->size;
  int iter,imgIdx;
  // decide where to start image indexing
  if (col1==0 && row1==0) {
    imgIdx = size*(size/2);
  }
  else if (col1==0 && row1!=0) {
    imgIdx = 0;
  }
  else if (col1!=0 && row1==0) {
    imgIdx = size*(size/2)+((size/2)*(size/2));
  }
  else if (col1!=0 && row1!=0) {
    imgIdx = size/2;
  }
  unsigned char r,g,b;
  float i,j;
  float xfrac,yfrac,x0,y0,x,y,xtmp;
  struct ppm_pixel pixel;
  maxIterations = tInfo->maxIterations;
  for (i=0;i<size/2;i++) {
    for (j=size/2;j<size;j++) {
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
      if(j==size-1){
        imgIdx=imgIdx+(size/2);
      }
      imgIdx++;
    }
  }
  free(tInfo);
  return NULL;
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
  (*threadInformation).size = size;
  (*threadInformation).maxIterations = maxIterations;
  (*threadInformation).pallete = pallete;
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
  (*threadInformation).size = size;
  (*threadInformation).maxIterations = maxIterations;
  (*threadInformation).pallete = pallete;
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
  (*threadInformation).size = size;
  (*threadInformation).maxIterations = maxIterations;
  (*threadInformation).pallete = pallete;
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
  (*threadInformation).size = size;
  (*threadInformation).maxIterations = maxIterations;
  (*threadInformation).pallete = pallete;
  (*threadInformation).imageData = image;
  pthread_create(&threads[3],NULL,computeImage,(void *) threadInformation);

  // join threads together
  int i;
  for (i=0;i<N;i++) {
    pthread_join(threads[i],NULL);
  }

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
