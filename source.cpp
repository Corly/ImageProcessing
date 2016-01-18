#include <stdio.h>
#include <stdlib.h>
#include <corona.h>
#include <string.h>
#include "exposure.h"

typedef unsigned char byte;

int main(int argc, char **argv) {

    corona::Image *image = corona::OpenImage(argv[1], corona::PF_R8G8B8A8);

    if (!image) {
        printf("Error opening image\n");
    }

    int width = image->getWidth();
    int height = image->getHeight();
    void *pixels = image->getPixels();

    printf("%d x %d\n", width, height);

    byte *p = (byte*) pixels;

    byte *red = (byte*) malloc(width * height);
    byte *green = (byte*) malloc(width * height);
    byte *blue = (byte*) malloc(width * height);
    byte *alpha = (byte*) malloc(width * height);
     
    byte *r = red;
    byte *g = green;
    byte *b = blue;
    byte *a = alpha;
     
    for (int i = 0; i < width * height; ++i) {
        *r++ = *p++;
        *g++ = *p++;
        *b++ = *p++;
        *a++ = *p++;
    }

    /*
    // exposure set
    double *y_yiq = (double*) malloc(sizeof(double) * width * height);
    double *i_yiq = (double*) malloc(sizeof(double) * width * height);
    double *q_yiq = (double*) malloc(sizeof(double) * width * height);

    rgb_to_yiq(red, green, blue, y_yiq, i_yiq, q_yiq, width, height);

    printf("%d\n", check_exposure(y_yiq, width, height));
    reglate_exposure(y_yiq, width, height, check_exposure(y_yiq, width, height));

    yiq_to_rgb(red, green, blue, y_yiq, i_yiq, q_yiq, width, height);
    */

    // white balancing
    byte *histogram_red = (byte*)malloc(sizeof(byte) * width * height); 
    byte *histogram_green = (byte*)malloc(sizeof(byte) * width * height); 
    byte *histogram_blue = (byte*)malloc(sizeof(byte) * width * height); 

    compute_color_histogram(red, histogram_red, width, height);
    compute_color_histogram(green, histogram_green, width, height);
    compute_color_histogram(blue, histogram_blue, width, height);

    histogram_stretch(red, histogram_red, width, height);
    histogram_stretch(green, histogram_green, width, height);
    histogram_stretch(blue, histogram_blue, width, height);

    p = (byte*)pixels;

    r = red;
    g = green;
    b = blue;
    a = alpha;

    for (int i = 0; i < width * height; ++i) {
        *p++ = *r++;
        *p++ = *g++;
        *p++ = *b++;
        *p++ = *a++;
    }

    corona::Image *new_image = corona::CreateImage(width, height, corona::PF_R8G8B8A8, pixels);

    corona::SaveImage("process_image.png", corona::FF_PNG, new_image);

    delete image;

    free(red);
    free(green);
    free(blue);
    free(alpha);

    return 0;
}
