#include <stdio.h>
#include <stdlib.h>
#include <corona.h>
#include <string.h>

typedef unsigned char byte;

int main(int argc, char **argv) {

    corona::Image *image = corona::OpenImage(argv[1], corona::PF_R8G8B8A8);

    if (!image) {
        printf("Error opening image\n");
    }

    int width = image->getWidth();
    int height = image->getHeight();
    void *pixels = image->getPixels();

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

    delete image;

    free(red);
    free(green);
    free(blue);
    free(alpha);

    return 0;
}
