#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "util.h"

uint8_t mandelbrot(double x0, double y0);
double scale(double x, double min, double max);

int main(){
    //This program prints the mandelbrot set to stdout as a .ppm image file
    // width and height can be modified below
    size_t width = 2048;
    size_t height = 2048;
    
    printf("P6 %ld %ld 255\n", width, height);

    uint8_t** data = array(uint8_t*, height);

    #pragma omp parallel for
    for(size_t y = 0; y < height; y++)
    {
        uint8_t* row = array(uint8_t, width * 3);
        for(size_t x = 0; x < width; x++){
            double fx = scale(x / (double)width, -2, 1);
            double fy = scale(y / (double)height, -1.5, 1.5);
            uint8_t value = mandelbrot(fx, fy);
            size_t i = x * 3;
            row[i++] = value;
            row[i++] = value;
            row[i++] = value;
        }
        data[y] = row;
    }

    for(size_t y = 0; y < height; y++)
        fwrite(data[y], 3, width, stdout);
}


double scale(double x, double min, double max){
    return x * (max - min) + min;
}

void square(double x, double xi, double* y, double* yi){
    *y = x * x - xi * xi;
    *yi = 2 * x * xi;
}

double lengthSq(double x, double xi){
    return x * x + xi * xi;
}

uint8_t mandelbrot(double x0, double y0){
    uint8_t i = 0;
    double x = x0;
    double y = y0;
    while(i < 255 && lengthSq(x, y) < 4){
        square(x, y, &x, &y);
        x += x0;
        y += y0;
        i++;
    }
    return i;
}