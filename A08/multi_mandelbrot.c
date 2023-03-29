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

  // begin computaiton of image, split processes
  pid_t pid;
  int child_status;
  pid = fork();
  if(pid==0) {
    ////////////// child 1 process ///////////////////
    printf("Launched child process: %d",getpid());
    printf("%d) Sub-image block: cols (%d,%d) to rows (0,%d)\n",
      pid,size/2,size,size/2);
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
    // exit child 1
    exit(0);
  }
  else {
    // parent process...spawn another child
    pid = fork();
    if (pid==0) {
      // child 2 process



    }
    else {
      // parent process...spawn another child
      pid = fork();
      if (pid==0){
        // child 3 process
      }
      else {
        // parent process
        printf("%d) Sub-image block: cols (0,%d) to rows (0,%d)\n",
          pid,size/2,size/2);
        int iter,imgIdx=0;
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
        // exit parent process
        exit(0);
      }

    }

  }

  // generate pallet
  // compute image
}
