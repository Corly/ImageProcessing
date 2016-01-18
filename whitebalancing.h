#ifndef WHITE_BALANCING_H
#define WHITE_BALANCING_H

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;

void compute_color_histogram(byte *color, byte *histogram, int width, int height)
{
    for (int i = 0; i < 256; ++i) {
        histogram[i] = 0;
    }

    for (int i = 0; i < width * height; ++i) {
        histogram[color[i]]++;
    }
}

void histogram_stretch(byte *color, byte *histogram, int width, int height)
{
    int low = 0;
    int high = 255;
    int local_low = 0;
    int local_high = 255;

    int five_percent_limit = ((float)(5 / 100)) * (width * height);

    int counter = 0;
    for (int i = 0; i < 256; ++i) {
        counter += histogram[i];
        if (counter > five_percent_limit) {
            local_low = i;
        }
    }

    counter = 0;
    for (int i = 255; i >= 0; --i) {
        counter += histogram[i];
        if (counter > five_percent_limit) {
            local_high = i;
        }
    }

    for (int i = 0; i < width * height; ++i) {
        color[i] = (color[i] - local_low) * ((high - low) /
                (local_high - local_low)) + low;
    }
}


#endif
