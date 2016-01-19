#ifndef SATURATION_H
#define SATURATION_H

#include <stdio.h>
#include <stdlib.h>
#include <corona.h>
#include <math.h>

#define UNDERSATURATION 1
#define SATURATION_OK  2
#define OVERSATURATION 3
#define DELTA 0.004


#define  Pr  0.299
#define  Pg  0.587
#define  Pb  0.114

typedef unsigned char byte;

double hue2rgb(double p, double  q, double  t)
{
    if(t < 0) t += 1;
    if(t > 1) t -= 1;
    if(t < 1/6){
        printf("1/6\n");
        return p + (q - p) * 6 * t;
    }
    if(t < 1/2) {
        printf("1/2\n");
        return q;
    }

    if(t < 2/3) {
        printf("1/3\n");
        return p + (q - p) * (2/3 - t) * 6;
    }
    return p;
}


void hslToRgb(double h, double s, double l, byte *r, byte *g, byte *b)
{
    printf("hsl: %lf, %lf, %lf\n", h, s, l);

    double R, G, B;
    if (s < DELTA) {
        R = G = B = l;
    }
    else {
        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;

        double offset = 1.0f / 3.0f;
        R = hue2rgb(p, q, h + offset);
        G = hue2rgb(p, q, h);
        B = hue2rgb(p, q, h - offset);
    }

    *r = R * 255;
    *g = G * 255;
    *b = B * 255;
    // printf("rgb: %lf, %lf, %lf\n", R, G, B);

    printf("rgb: %d, %d, %d\n", *r, *g, *b);

}

double getMax(double a, double b, double c) 
{

    if (a < b) {
        if ( b < c) {
            return c;
        }
        else {
            return b;
        }
    }

    return a;
}

double getMin(double a, double b, double c) 
{

    if (a < b) {
        if ( c < a) {
            return c;
        }
        else {
            return a;
        }
    }
    return b;
}

void rgbToHsl(byte r, byte g, byte b, double *h, double *s, double *l)
{
    double R, G, B;

    R = (double) r / 255;
    G = (double) g / 255;
    B = (double) b / 255;

    double max = getMax(R, G, B);
    double min = getMin(R, G, B);

    *l = (max + min) / 2;

    if (max == min) {
        *h = *s = 0; //acromatic
    }
    else {
        double d = max - min;
        *s = *l > 0.5 ? d / (2 - max - min) : d / (max + min);
        
        if (max == R) {
            *h = (double) (G - B) / d + (G < B ? 6 : 0); 
        }

        if (max == G) {
            *h = (double) (B - R) / d + 2;
        }

        if (max == B) {
            *h = (double) (R - G) / d + 4;
        }
        
        // printf ("before: %lf\n", *h);
        *h /= 6.0f;
        // printf ("after: %lf\n", *h);

    }   
    // printf("hsl: %lf, %lf, %lf\n", *h, *s, *l);

}

int check_saturation(double *h, int width, int height)
{
    double first_threshold = 0.33f;
    double second_threshold = 0.66f;

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

void changeSaturation(double *s, double change) {

    double first_threshold = 0.33f;
    double second_threshold = 0.66f;

    if (*s < first_threshold || *s > second_threshold) {
        *s += change;
        return;
    }
    else {
        *s += change / 4;
        return;
    }

}

void correct_saturation(byte *red, byte *green, byte *blue, int width, int height) 
{


    double *h = (double*)malloc(sizeof(double) * width * height); 
    double *s = (double*)malloc(sizeof(double) * width * height); 
    double *l = (double*)malloc(sizeof(double) * width * height); 

    for (int i = 0; i < width * height; ++i) {
        rgbToHsl(red[i], green[i], blue[i], &h[i], &s[i], &l[i]);
        //printf("hsl before: %lf, %lf, %lf\n", h[i], s[i], l[i]);

    }

    int i = 85;
    printf("hsl before: %lf, %lf, %lf\n", h[i], s[i], l[i]);


    int k = check_saturation(s, width, height);

    // if (k == UNDERSATURATION) {
    //     for (int i = 0; i < width * height; ++i) {
    //         changeSaturation(&s[i], 0.1);
    //     }
    // } 
    // else if(k == OVERSATURATION) {
    //     for (int i = 0; i < width * height; ++i) {
    //         changeSaturation(&s[i], -0.1);
    //     }
    // }

    printf("hsl: %lf, %lf, %lf\n", h[i], s[i], l[i]);

    for (int i = 0; i < width * height; ++i) {
        // printf("before: %d, %d, %d\n", red[i], green[i], blue[i]);
        //printf("hsl: %lf, %lf, %lf\n", h[i], s[i], l[i]);

        hslToRgb(h[i], s[i], l[i], &red[i], &green[i], &blue[i]);

        // printf("after: %d, %d, %d\n", red[i], green[i], blue[i]);
    }

}




#endif
