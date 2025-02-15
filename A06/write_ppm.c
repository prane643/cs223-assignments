/*----------------------------------------------
 * Author: Pranav Rane
 * Date: Feb 28
 * Description
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) {
    FILE * infile;
    infile = fopen(filename,"wb");
    // write header
    fprintf(infile,"P6\n#testing write file\n%d %d\n255\n",h,w);
    // write in data
    fwrite(pixels,sizeof(struct ppm_pixel),w*h,infile);
    fclose(infile);
}

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}
