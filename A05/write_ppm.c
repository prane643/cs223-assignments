/*----------------------------------------------
 * Author: 
 * Date: 
 * Description
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    FILE * infile;
    infile = fopen(filename,"wb");
    // test input
    //printf("\n(%d,%d,%d)\n",pixels[1].red,pixels[1].green,pixels[1].blue);
    // write header
    fprintf(infile,"P6 #testing write file 4 4 255");
    // write in data
    fwrite(pixels,sizeof(struct ppm_pixel),w*h,infile);
    fclose(infile);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
