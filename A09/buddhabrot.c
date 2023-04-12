/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Apr 11
 * Description: Compute buddhabrot with counts MT
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
#include <math.h>

// intialize global variable for max count
static float maxCount = 0;
// create barrier
pthread_barrier_t mybarrier;
// create mutex
pthread_mutex_t mutex;

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
  int tid;
  int *membership;
  int *count;
  struct ppm_pixel* imageData;
};

void *computeImage(void *arg) {
  struct threadInformation *tInfo;
  tInfo = (struct threadInformation*)arg;
  // define some variables
  int col1,col2,row1,row2,size,tid;
  int maxIterations,istart,ifinish,jstart,jfinish;
  float xmin,xmax,ymin,ymax;
  struct ppm_pixel* image;
  //unsigned char *pallete;
  int *membership;
  int* count;
  // get information from struct
  xmin = tInfo->xmin;
  xmax = tInfo->xmax;
  ymin = tInfo->ymin;
  ymax = tInfo->ymax;
  col1 = tInfo->col1;
  col2 = tInfo->col2;
  row1 = tInfo->row1;
  row2 = tInfo->row2;
  tid = tInfo->tid;
  count = tInfo->count;
  membership = tInfo->membership;
  image = tInfo->imageData;
  // identify thread
  printf("\nThread %d) Sub-image block: cols (%d,%d) to rows (%d,%d)",
    tid,col1,col2,row1,row2);
  size = tInfo->size;
  int iter,imgIdx;
  // decide where to start image indexing
  if (col1==0 && row1!=0) {
    imgIdx = size*(size/2);
    istart = size/2;
    jstart = 0;
    ifinish = size;
    jfinish = size/2;
  }
  else if (col1==0 && row1==0) {
    imgIdx = 0;
    istart = 0;
    ifinish = size/2;
    jstart = 0;
    jfinish = size/2;
  }
  else if (col1!=0 && row1!=0) {
    imgIdx = size*(size/2)+(size/2);
    istart = size/2;
    ifinish = size;
    jstart = size/2;
    jfinish = size;
  }
  else if (col1!=0 && row1==0) {
    imgIdx = size/2;
    istart = 0;
    ifinish = size/2;
    jstart = size/2;
    jfinish = size;
  }
  unsigned char r,g,b;
  int i,j;
  float xfrac,yfrac,x0,y0,x,y,xtmp;
  struct ppm_pixel pixel;
  maxIterations = tInfo->maxIterations;
  int imgIdx1 = imgIdx, imgIdx2;
  // stage 1: membership
  for (i=istart;i<ifinish;i++) {
    for (j=jstart;j<jfinish;j++) {
      xfrac = (float)j/size;
      yfrac = (float)i/size;
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
        // set membership to false
        membership[imgIdx] = 0;
      }
      else {
        // set membership to true
        membership[imgIdx] = 1;
      }
      if(j==jfinish-1){
        imgIdx=imgIdx+(size/2);
      }
      imgIdx++;
    }
  }

  // start stage 2: compute visited counts
  int yrow,xcol;
  for (i=istart;i<ifinish;i++) {
   for (j=jstart;j<jfinish;j++) {
      if (membership[imgIdx1]==0) {
        xfrac = (float)j/size;
        yfrac = (float)i/size;
        x0 = xmin + xfrac * (xmax - xmin);
        y0 = ymin + yfrac * (ymax - ymin);
        x = 0;
        y = 0;
        while (x*x + y*y < 2*2) {
          xtmp = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xtmp;
          yrow = round(size * (y - ymin)/(ymax - ymin));
          xcol = round(size * (x - xmin)/(xmax - xmin));
          if (yrow < 0 || yrow >= size) {
            continue; // out of range
          } 
          if (xcol < 0 || xcol >= size) {
            continue; // out of range
          }
          imgIdx2 = yrow*size + xcol;
          // lock mutex
          pthread_mutex_lock(&mutex);
          count[imgIdx2] = count[imgIdx2]+1;
          //update max count
          if (count[imgIdx2]>maxCount) {
            maxCount = count[imgIdx2];
          }
          // unlock mutex
          pthread_mutex_unlock(&mutex);
        }
      }
      if(j==jfinish-1){
        imgIdx1=imgIdx1+(size/2);
      }
      imgIdx1++;
   }
  }
  // use barrier to wait for all counts to finish
  printf("\n finished counts for thread %d",tid);
  pthread_barrier_wait(&mybarrier);
  printf("\n finished all counts");

  // start stage 3: compute colors
  printf("\n compute colors for thread %d",tid);
  float gamma = 0.681;
  float factor = 1.0/gamma;
  float value;
  struct ppm_pixel color;
  int i1,j1;
  for (i1=istart;i1<ifinish;i1++) {
    for (j1=jstart;j1<jfinish;j1++) {
        value = 0;
        if (count[i1*size+j1]>0) {
            value = log(count[i1*size+j1]) / log(maxCount);
            value = pow(value, factor);
            color.red = value * 255;
            color.green = value * 255;
            color.blue = value * 255;
            image[i1*size+j1] = color;
        }
    }
  }
  // free struct
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

  pthread_barrier_init(&mybarrier,NULL,4);
  pthread_mutex_init(&mutex,NULL);

  // create memory to store image
  struct ppm_pixel *image = malloc(sizeof(struct ppm_pixel)*size*size);
  // create  memory to store membership array
  int *membership = malloc(sizeof(int)*size*size);
  // create memory for count array
  int *count = malloc(sizeof(int)*size*size);
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
  (*threadInformation).xmax = xmax;
  (*threadInformation).ymin = ymin;
  (*threadInformation).ymax = ymax;
  (*threadInformation).col1 = 0;
  (*threadInformation).col2 = size/2;
  (*threadInformation).row1 = size/2;
  (*threadInformation).row2 = size;
  (*threadInformation).size = size;
  (*threadInformation).maxIterations = maxIterations;
  (*threadInformation).tid = 0;
  (*threadInformation).membership = membership;
  (*threadInformation).count = count;
  (*threadInformation).imageData = image;
  pthread_create(&threads[0],NULL,computeImage,(void *) threadInformation);

  // run thread 2
  threadInformation = malloc(sizeof(struct threadInformation));
  (*threadInformation).xmin = xmin;
  (*threadInformation).xmax = xmax;
  (*threadInformation).ymin = ymin;
  (*threadInformation).ymax = ymax;
  (*threadInformation).col1 = size/2;
  (*threadInformation).col2 = size;
  (*threadInformation).row1 = size/2;
  (*threadInformation).row2 = size;
  (*threadInformation).size = size;
  (*threadInformation).maxIterations = maxIterations;
  (*threadInformation).tid = 1;
  (*threadInformation).membership = membership;
  (*threadInformation).count = count;
  (*threadInformation).imageData = image;
  pthread_create(&threads[1],NULL,computeImage,(void *) threadInformation);

  // run thread 3
  threadInformation = malloc(sizeof(struct threadInformation));
  (*threadInformation).xmin = xmin;
  (*threadInformation).xmax = xmax;
  (*threadInformation).ymin = ymin;
  (*threadInformation).ymax = ymax;
  (*threadInformation).col1 = 0;
  (*threadInformation).col2 = size/2;
  (*threadInformation).row1 = 0;
  (*threadInformation).row2 = size/2;
  (*threadInformation).size = size;
  (*threadInformation).maxIterations = maxIterations;
  (*threadInformation).tid = 2;
  (*threadInformation).membership = membership;
  (*threadInformation).count = count;
  (*threadInformation).imageData = image;
  pthread_create(&threads[2],NULL,computeImage,(void *) threadInformation);

  // run thread 4
  threadInformation = malloc(sizeof(struct threadInformation));
  (*threadInformation).xmin = xmin;
  (*threadInformation).xmax = xmax;
  (*threadInformation).ymin = ymin;
  (*threadInformation).ymax = ymax;
  (*threadInformation).col1 = size/2;
  (*threadInformation).col2 = size;
  (*threadInformation).row1 = 0;
  (*threadInformation).row2 = size/2;
  (*threadInformation).size = size;
  (*threadInformation).maxIterations = maxIterations;
  (*threadInformation).tid = 3;
  (*threadInformation).membership = membership;
  (*threadInformation).count = count;
  (*threadInformation).imageData = image;
  pthread_create(&threads[3],NULL,computeImage,(void *) threadInformation);

  // join threads together
  int i;
  for (i=0;i<N;i++) {
    pthread_join(threads[i],NULL);
    printf("\nThread %d) finished",i);
  }

  // calculate computation time
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("\nComputed mandelbrot set (%dx%d) in %g seconds\n",size,size,timer);

  // write image
  char buffer[100];
  sprintf(buffer,"buddhabrot-%d-%ld.ppm",size,time(0));
  write_ppm(buffer,image,size,size);

  // destroy mutex and barrier
  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&mybarrier);

  // free memory
  free(membership);
  free(count);
  free(image);
  free(threads);
  return 0;
}
