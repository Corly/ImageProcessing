#ifndef BARREL_H
#define BARREL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

typedef unsigned char byte;

void apply_barrel_correction(byte *pixels, int width, int height)
{
    double param_a = -0.007715;
    double param_b = 0.026731;
    double param_c = 0.0;
    double param_d = 1.0 - param_a - param_b - param_c;

    byte *pixels_copy = (byte*)malloc(sizeof(byte) * width * height);
    for (int i = 0; i < width * height; ++i) {
        pixels_copy[i] = pixels[i];
    }

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            int radius = MIN(width, height) / 2;

            double center_correct_x = (width - 1) / 2.0f;
            double center_correct_y = (height - 1) / 2.0f;

            double delta_x = (x - center_correct_x) / radius;
            double delta_y = (y - center_correct_y) / radius;

            // distance of correct image
            double d_corr = sqrt(delta_x * delta_x + delta_y * delta_y);

            // distance of incorrect image (using formula)
            double d_incorr = (param_a * d_corr * d_corr * d_corr +
                param_b * d_corr * d_corr + param_c * d_corr + param_d) * d_corr;

            // get factor
            double factor = abs(d_corr / d_incorr);

            int src_x = center_correct_x + (delta_x * factor * radius);
            int src_y = center_correct_y + (delta_y * factor * radius);

            if (src_x >= 0 && src_y >= 0 && src_x < width && src_y < height) {
                printf("%d %d\n", pixels[y * width + x], pixels_copy[src_y * width + src_x]);
                pixels[y * width + x] = pixels_copy[src_y * width + src_x];  
            }
        }
    }
}

void barrel_correction(byte *pixels, int width, int height, float strength, float zoom)
{
    byte *pixels_copy = (byte*)malloc(sizeof(byte) * width * height);
    for (int i = 0; i < width * height; ++i) {
        pixels_copy[i] = pixels[i];
    }

    int half_width = width / 2;
    int half_height = height / 2;

    float correction_radius = sqrt(width * width + height * height) / strength;

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            int new_x = x - half_width;
            int new_y = y - half_height;

            float distance = sqrt(new_x * new_x + new_y * new_y);
            float r = distance / correction_radius;

            float theta = 1.0f;
            if (r != 0) {
                theta = atan(r) / r;
            }

            int source_x = half_width + theta * new_x * zoom;
            int source_y = half_height + theta * new_y * zoom;

            if (source_x >= 0 && source_y >= 0 && source_x < width && source_y < height) {
                pixels[y * width + x] = pixels_copy[source_y * width + source_x];  
            }
        }
    }

    
}

#endif
