#####################################################
> Project Title:    EE569 - Homework Assignment 2
> Date:             02-12-2019
> Author:           Spencer McDonough
> ID:               6341617166
> email:            shmcdono@usc.edu
> OS:               OSX
> Compiler:         g++
#####################################################

# TODO
1. [X] sobel ED: create step by step outline 
2. [X] sobel ED: pseudocode comment map 
3. [X] sobel ED: develop, test, modify 
4. [] canny ED: create step by step outline
5. [] canny ED: pseudocode comment map
6. [] canny ED: develop, test, modify

# references
dithering matrix: https://www.youtube.com/watch?v=K7RCH5yyVVE
jeff: https://github.com/J-pcy/Digital-Image-Processing

# Compile and Execution Instructions

Problem 1.a)
$ g++ colorToGray.cpp -o colorToGray
$ g++ gaussianDenoise.cpp -o gaussianDenoise
$ g++ sobel.cpp -o sobel
$ ./colorToGray HW2_images/Tiger.raw HW2_images/tigerGray.raw
$ ./gaussianDenoise HW2_images/tigerGray.raw HW2_images/tigerDenoise.raw 1 481 321 3
$ ./sobel HW2_images/tigerDenoise.raw HW2_images/sobelTiger.raw 1 481 321 3 128 20

Problem 1.b)
$ g++ -g -Wall -I/opt/local/include/ canny.cpp -o canny
