/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Mar 29
 * Description: Using 4 processes to compute mandelbrot set
 ---------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
  int size = 2000;
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

  // generate pallette
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

  // create shared memory
  int shmid;
  shmid = shmget(IPC_PRIVATE,sizeof(struct ppm_pixel)*size*size,
    0644 | IPC_CREAT);
  if (shmid==-1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }

  // create pointer to shared memory
  struct ppm_pixel* image = shmat(shmid,NULL,0);
  if (image==(void*)-1) {
    perror("Error: cannot access shared memory");
    exit(1);
  }

  // set up timer
  double timer;
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);

  // begin computaiton of image, split processes
  pid_t pid;
  int child_status;
  pid = fork();
  if(pid==0) {
    ////////////// child 2 process ///////////////////
    printf("Launched child process: %d\n",getpid());
    printf("%d) Sub-image block: cols (%d,%d) to rows (0,%d)\n",
      getpid(),size/2,size,size/2);
    int iter,imgIdx=size/2;
    unsigned char r,g,b;
    float i,j;
    float xfrac,yfrac,x0,y0,x,y,xtmp;
    struct ppm_pixel pixel;
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
    // exit child 2
    shmdt(image);
    free(pallete);
    printf("Child process complete: %d\n",getpid());
    exit(0);
  }
  else {
    // parent process...spawn another child
    pid = fork();
    if (pid==0) {
      ////////////// child 3 process ///////////////////
      printf("Launched child process: %d\n",getpid());
      printf("%d) Sub-image block: cols (0,%d) to rows (%d,%d)\n",
        getpid(),size/2,size/2,size);
      int iter,imgIdx=2*(size/2)*(size/2)+1;
      unsigned char r,g,b;
      float i,j;
      float xfrac,yfrac,x0,y0,x,y,xtmp;
      struct ppm_pixel pixel;
      for (i=size/2;i<size;i++) {
        for (j=0;j<size/2;j++) {
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
          if(j==(size/2)-1){
            imgIdx=imgIdx+(size/2);
          }
          imgIdx++;
        }
      }
      // exit child 3 process
      shmdt(image);
      free(pallete);
      printf("Child process complete: %d\n",getpid());
      exit(0);
    }
    else {
      // parent process...spawn another child
      pid = fork();
      if (pid==0) {
        ////////////// child 4 process ///////////////////
        printf("Launched child process: %d\n",getpid());
        printf("%d) Sub-image block: cols (%d,%d) to rows (%d,%d)\n",
          getpid(),size/2,size,size/2,size);
        int iter,imgIdx=2*(size/2)*(size/2)+(size/2);
        unsigned char r,g,b;
        float i,j;
        float xfrac,yfrac,x0,y0,x,y,xtmp;
        struct ppm_pixel pixel;
        for (i=size/2;i<size;i++) {
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
        // exit child 4 process
        shmdt(image);
        free(pallete);
        printf("Child process complete: %d\n",getpid());
        exit(0);
      }
      else {
        // parent process
        printf("Launched child process: %d\n",getpid());
        printf("%d) Sub-image block: cols (0,%d) to rows (0,%d)\n",
          getpid(),size/2,size/2);
        int iter,imgIdx=0, status=0;
        unsigned char r,g,b;
        float i,j;
        float xfrac,yfrac,x0,y0,x,y,xtmp;
        struct ppm_pixel pixel;
        for (i=0;i<size/2;i++) {
          for (j=0;j<size/2;j++) {
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
            if(j==(size/2)-1){
              imgIdx=imgIdx+(size/2);
            }
            imgIdx++;
          }
        }
        printf("Child process complete: %d\n",getpid());
        // exit parent process after waiting
        while (wait(&status)>0);
        // calculate time
        gettimeofday(&tend, NULL);
        timer = tend.tv_sec - tstart.tv_sec +
          (tend.tv_usec - tstart.tv_usec)/1.e6;
        printf("Computed mandelbrot set (%dx%d) in"
          " %g seconds\n",size,size,timer);
        // write file
        char buffer[100];
        sprintf(buffer,"multi-mandelbrot-%d-%ld.ppm",size,time(0));
        printf("Writing file: %s\n",buffer);
        write_ppm(buffer,image,size,size);
        // detatch and remove memory
        shmdt(image);
        shmctl(shmid,IPC_RMID,0);
        free(pallete);
        exit(0);
      }
    }
  }
  return 0;
}
