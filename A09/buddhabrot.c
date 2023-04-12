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
  int size;
  int maxIterations;
  int tid;
  unsigned char *pallete;
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
  unsigned char *pallete;
  int *membership;
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
  pallete = tInfo->pallete;
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
  float i,j;
  float xfrac,yfrac,x0,y0,x,y,xtmp;
  struct ppm_pixel pixel;
  maxIterations = tInfo->maxIterations;
  int imgIdx1 = imgIdx, imgIdx2;
  for (i=istart;i<ifinish;i++) {
    for (j=jstart;j<jfinish;j++) {
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
        // set membership to false
        membership[imgIdx] = 0;
        /*
        r = pallete[iter*3];
        g = pallete[iter*3+1];
        b = pallete[iter*3+2];
        pixel.red = r;
        pixel.green = g;
        pixel.blue = b;
        */
      }
      else {
        // set membership to true
        membership[imgIdx] = 1;
        /*
        pixel.red = 0;
        pixel.green = 0;
        pixel.blue = 0;
        */
      }
      //image[imgIdx] = pixel;
      if(j==jfinish-1){
        imgIdx=imgIdx+(size/2);
      }
      imgIdx++;
    }
  }

  // start stage 2: compute visited counts
  int yrow,xcol;
  int* count;
  for (i=istart;i<ifinish;i++) {
   for (j=jstart;j<jfinish;j++) {
      if (membership[imgIdx1]==1) {
        xfrac = j/size;
        yfrac = i/size;
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
          // convert yrow xcol to imgIdx2
          imgIdx2 = yrow*size + xcol;
          count[imgIdx2] = count[imgIdx1]+1;
          //update max count
          if (count[imgIdx2]>maxCount) {
            maxCount = count[imgIdx2];
          }

        }
      }
      if(j==jfinish-1){
        imgIdx=imgIdx+(size/2);
      }
      imgIdx++;
   }
  }


  free(tInfo);
  return NULL;
}



 // intialize global variable for max count
 static int maxCount = 0;



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

  // todo: your code here
  // compute image

 

}
