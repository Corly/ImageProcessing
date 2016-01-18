#ifndef SATURATION_H
#define SATURATION_H

#include <stdio.h>
#include <stdlib.h>
#include <corona.h>
#include <math.h>

#define UNDERSATURATION  1
#define SATURATION_OK  2
#define OVERSATURATION 3

#define  Pr  0.299
#define  Pg  0.587
#define  Pb  0.114

typedef unsigned char byte;

void changeSaturation(byte *R, byte *G, byte *B, double change) {

    double  P = sqrt(
            (*R) * (*R) * Pr +
            (*G) * (*G) * Pg +
            (*B) * (*B) * Pb ) ;

    *R = P + ((*R) - P) * change;
    *G = P + ((*G) - P) * change;
    *B = P + ((*B) - P) * change; 
}

int check_saturation(int *h, int width, int height)
{
    double first_threshold = (255) * 0.33f;
    double second_threshold = (255) * 0.66f;

    int nr1 = 0;
    int nr2 = 0;
    int nr3 = 0;

    for (int i = 0; i < width * height; ++i) {
        if (h[i] <= first_threshold) {
            nr1++;
        }
        else if (first_threshold < h[i] && h[i] < second_threshold) {
            nr2++;
        }
        else {
            nr3++;
        }
    }

    if (nr1 > nr2 && nr1 > nr3) {
        printf("%d\n", UNDERSATURATION);

        return UNDERSATURATION;
    }
    else if (nr2 > nr1 && nr2 > nr3) {
        return SATURATION_OK;
    }
    else {
        printf("%d\n", OVERSATURATION);

        return OVERSATURATION;
    }
}

int* average(int *h_red, int *h_green, int *h_blue, int width, int height) 
{
    int *histogram = (int*) malloc(sizeof(int) * width * height); 

    for (int i = 0; i < width * height; ++i) {
        histogram[i] = (h_red[i] + h_green[i] + h_green[i]) / 3;
    }

    return histogram;
}

void correct_saturation(byte *red, byte *green, byte *blue,
                       int *h_red, int *h_green, int *h_blue,
                       int width, int height) 
{

    int *h = average(h_red, h_green, h_blue, width, height);

    int k = check_saturation(h, width, height);

    if (k == UNDERSATURATION) {
        for (int i = 0; i < width * height; ++i) {
            changeSaturation(&red[i], &green[i], &blue[i], 1.05);
        }
    } 
    else if(k == OVERSATURATION) {
        for (int i = 0; i < width * height; ++i) {
            changeSaturation(&red[i], &green[i], &blue[i], 0.95);
        }
    }

}




#endif
