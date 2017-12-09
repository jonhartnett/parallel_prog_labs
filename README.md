# Parallel Programming Labs
### Jonathan Hartnett

The following repository contains my submissions for the parallel-programming special study labs. All solutions tested on Ubuntu.

1. Histogram  
Simple introduction to parallel programming with OpenMP. Reads data and configuration from stdin. 
Prints a histogram to stdout. No known issues.

2. Monte  
Calculates PI using a PRNG by throwing "darts" at a circular "dartboard" and calculating the number that land on target.
Reads number of tosses to use from stdin (larger numbers lead to more accurate estimates). 
Prints PI estimate and the error to stdout. No known issues.

3. Mandelbrot  
Generates an image of the mandelbrot set. Prints the result as a ppm image to stdout. 
Intended to be used with a command line pipe to create a file. Currently, image dimensions are hard-coded into the program.
Beyond this, no known issues.

4. N-Body  
Performs an n-body gravity simulation. Reads body parameters and simulation configuration from stdin. 
Writes resulting body parameters after the simulation to stdout. Uses standard metric units for all parameters.
There are some different options for simulation types in the main file. Default is reduced-runge-kutta.
No known issues.
