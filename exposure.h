#ifndef EXPOSURE_H
#define EXPOSURE_H

#include <stdio.h>
#include <stdlib.h>
#include <corona.h>

#define POS (k * width + j)
#define DELTA 0.004
#define UNDEREXPOSURE  1
#define EXPOSURE_OK  2
#define OVEREXPOSURE 3

struct hist_value {
    double value;
    int count;
};

typedef unsigned char byte;

// transform rgb to yiq
void rgb_to_yiq(byte *r, byte *g, byte *b, double *y, double *i, double *q,
                    int width, int height)
{
    //printf("%d %d %d\n", r[POS], g[POS], b[POS]);
    for (int k = 0; k < height; ++k) {
        for (int j = 0; j < width; ++j) {
            y[POS] = 0.299 * ((double)r[POS]/255) + 0.587 * ((double)g[POS]/255)
                        + 0.114 * ((double)b[POS]/255);
            i[POS] = 0.596 * ((double)r[POS]/255) - 0.274 * ((double)g[POS]/255) 
                        - 0.322 * ((double)b[POS]/255);
            q[POS] = 0.211 * ((double)r[POS]/255) - 0.523 * ((double)g[POS]/255)
                        + 0.312 * ((double)b[POS]/255);
        }
    }
}

// transform yiq to rgb
void yiq_to_rgb(byte *r, byte *g, byte *b, double *y, double *i, double *q,
                    int width, int height)
{
    for (int k = 0; k < height; ++k) {
        for (int j = 0; j < width; ++j) {
            r[POS] = 255 * (1 * y[POS] + 0.956 * i[POS] + 0.621 * q[POS]);
            g[POS] = 255 * (1 * y[POS] - 0.272 * i[POS] - 0.647 * q[POS]);
            b[POS] = 255 * (1 * y[POS] - 1.106 * i[POS] + 1.703 * q[POS]);
        }
    }
}

int compute_histogram(double *y, hist_value *histogram, int width, int height)
{
    //hist_value *histogram = (hist_value *)malloc(256 * sizeof(hist_value)); 
    int index = 0;

    for (int i = 0; i < width * height; ++i) {
        bool ok = false;
        for (int j = 0; j < index; ++j) {
            if (abs(y[i] - histogram[j].value) < DELTA) {
                histogram[j].count++;
                ok = true;
            }
        }
        
        if (!ok) {
            histogram[index].value = y[i];
            histogram[index].count = 1;
            index++;
        }
    }

    for (int i = 0; i < index - 1; ++i) {
        for (int j = i + 1; j < index; ++j) {
            if (histogram[i].value > histogram[j].value) {
                hist_value aux = histogram[i];
                histogram[i] = histogram[j];
                histogram[j] = aux;
            }
        }
    }

    return index;
}

int check_exposure(hist_value *histogram, int len)
{
    int nr1 = 0;
    int nr2 = 0;
    int nr3 = 0;

    for (int i = 0; i < len; ++i) {
        if (histogram[i].value <= 0.33) {
            nr1 += histogram[i].count;
        }
        else if (0.33 < histogram[i].value && histogram[i].value < 0.66) {
            nr2 += histogram[i].count;
        }
        else {
            nr3 += histogram[i].count;
        }
    }

    if (nr1 > nr2 && nr1 > nr3) {
        return UNDEREXPOSURE;
    }
    else if (nr2 > nr1 && nr2 > nr3) {
        return EXPOSURE_OK;
    }
    else {
        return OVEREXPOSURE;
    }
}

void reglate_exposure(double *y, int width, int height, int type)
{
    if (type == EXPOSURE_OK) {
        return;
    }
    else if (type == UNDEREXPOSURE) {
        for (int i = 0; i < width * height; ++i) {
            if (y[i] < 0.33) {
                y[i] += 0.053;
            }
            else if (y[i] > 0.33 && y[i] < 0.66) {
                y[i] += 0.01;
            }
        }
    }
    else if (type == OVEREXPOSURE) {
        for (int i = 0; i < width * height; ++i) {
            if (y[i] > 0.66) {
                y[i] -= 0.053;
            }
            else if (y[i] > 0.33 && y[i] < 0.66) {
                y[i] -= 0.01;
            }
        }
    }
}

#endif
