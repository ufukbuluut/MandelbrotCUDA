#include "mandelbrot_sequential.h"
#include <iostream>
#include <cmath>



int* mendelbrot_sequential(int nrows, int ncols, int max_iter, double startX, double startY, double zoom)
{
    int* result = new int[nrows * ncols];

    for (int y_idx = 0; y_idx < nrows; ++y_idx) {
        for (int x_idx = 0; x_idx < ncols; ++x_idx) {
            int l = x_idx + y_idx * ncols;

            // Calculate coordinates in the complex plane
            double x = ((x_idx - ncols / 2) * zoom + startX) / ncols * 3.5 - 0.75;
            double y = ((y_idx - nrows / 2) * zoom + startY) / nrows * 2.0;
            double re = x, im = y;

            // Mandelbrot set iteration
            for (int i = 1; i < max_iter; ++i) {
                if (re * re + im * im >= 4) {
                    result[l] = i;
                    break;
                }
                double reTemp = re * re - im * im + x;
                im = 2 * re * im + y;
                re = reTemp;
            }

            // Set result to 0 if within the Mandelbrot set
            if (result[l] == 0) {
                result[l] = 0;
            }
        }
    }

    return result;
}


