#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "util.h"

uint8_t mandelbrot(double x0, double y0);
double scale(double x, double min, double max);

int main(){
    //This program prints the mandelbrot set to stdout as a .ppm image file

    size_t width = 1024;
    size_t height = 1024;
    printf("P6 %ld %ld 255\n", width, height);

    uint8_t* data = array(uint8_t, width * height * 3);

#pragma omp parallel for
    for(int y = 0; y < height; y++)
    for(int x = 0; x < width; x++){
        double fx = scale(x / (double)width, -2, 1);
        double fy = scale(y / (double)height, -1.5, 1.5);
        uint8_t value = mandelbrot(fx, fy);
        size_t i = (x + y * width) * 3;
        data[i++] = value;
        data[i++] = value;
        data[i++] = value;
    }

    fwrite(data, 3, width * height, stdout);
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