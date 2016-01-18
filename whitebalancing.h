#ifndef WHITE_BALANCING_H
#define WHITE_BALANCING_H

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char byte;

void compute_color_histogram(byte *color, int *histogram, int width, int height)
{
    for (int i = 0; i < 256; ++i) {
        histogram[i] = 0;
    }

    for (int i = 0; i < width * height; ++i) {
        histogram[color[i]]++;
    }
}

void get_histogram_limits(int *histogram, int width, int height,
                            int *local_low, int *local_high)
{
    int five_percent_limit = 0.05f * width * height;

    int counter = 0;
    for (int i = 0; i < 256; ++i) {
        counter += histogram[i];
        if (counter > five_percent_limit) {
            *local_low = i;
            break;
        }
    }

    counter = 0;
    for (int i = 255; i >= 0; --i) {
        counter += histogram[i];
        if (counter > five_percent_limit) {
            *local_high = i;
            break;
        }
    }

}

void histogram_stretch(byte *red, byte *green, byte *blue,
                       int *h_red, int *h_green, int *h_blue,
                       int width, int height)
{
    int low = 0;
    int high = 255;
    int local_low_red = 0;
    int local_high_red = 255;
    int local_low_green = 0;
    int local_high_green = 255;
    int local_low_blue = 0;
    int local_high_blue = 255;
    
    get_histogram_limits(h_red, width, height, &local_low_red, &local_high_red);
    get_histogram_limits(h_green, width, height, &local_low_green, &local_high_green);
    get_histogram_limits(h_blue, width, height, &local_low_blue, &local_high_blue);

    int local_low_avg = (local_low_red + local_low_blue + local_low_green) / 3;
    int local_high_avg = (local_high_red + local_high_blue + local_high_green) / 3;

    for (int i = 0; i < width * height; ++i) {
        if (red[i] > local_low_red) {
            red[i] = (red[i] - local_low_red) * ((high - low) /
                    (local_high_red - local_low_red)) + low;
        }
        if (green[i] > local_low_green) {
            green[i] = (green[i] - local_low_green) * ((high - low) /
                    (local_high_green - local_low_green)) + low;
        }
        if (blue[i] > local_low_blue) {
            blue[i] = (blue[i] - local_low_blue) * ((high - low) /
                    (local_high_blue - local_low_blue)) + low;
        }
    }
}


#endif
