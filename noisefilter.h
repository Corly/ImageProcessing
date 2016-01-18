#ifndef NOISE_FILTER_H
#define NOISE_FILTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

byte *apply_filter(byte *mat, int image_width, int image_height,
                        int window_width, int window_height) {
    byte *filtered_mat = (byte*)malloc(image_width * image_height);
    byte *window = (byte*)malloc(window_width * window_height);
    int window_size = window_width * window_height;

    memcpy(filtered_mat, mat, image_width * image_height);

    int edgex = window_width / 2;
    int edgey = window_height / 2;

    for (int y = edgey; y <= image_height - edgey; ++y) {
        for (int x = edgex; x <= image_width - edgex; ++x) {
            int i = 0;
            for (int fx = 0; fx < window_width; ++fx) {
                for (int fy = 0; fy < window_height; ++fy) {
                    window[i] = mat[(x + fx - edgex) + (y + fy - edgey) * image_width];
                    ++i;
                }
                // sort the window
                for (int it1 = 0; it1 < window_size - 1; ++it1) {
                    for (int it2 = it1 + 1; it2 < window_size; ++it2) {
                        if (window[it1] > window[it2]) {
                            int aux = window[it1];
                            window[it1] = window[it2];
                            window[it2] = aux;
                        }
                    }
                }
            }

            filtered_mat[y * image_width + x] = window[window_size / 2];
        }
    }

    free(window);
    free(mat);

    return filtered_mat;
}

#endif
