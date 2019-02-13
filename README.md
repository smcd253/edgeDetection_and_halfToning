#####################################################
> Project Title:    EE569 - Homework Assignment 2
> Date:             02-12-2019
> Author:           Spencer McDonough
> ID:               6341617166
> email:            shmcdono@usc.edu
> OS:               OSX, Ubuntu
> Compiler:         g++
#####################################################

# references
dithering matrix: https://www.youtube.com/watch?v=K7RCH5yyVVE
opencv canny edge detector: https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html
error diffusion: https://engineering.purdue.edu/~bouman/ece637/notes/pdf/Halftoning.pdf

# Compile and Execution Instructions

Problem 1.a)
$ g++ colorToGray.cpp -o colorToGray
$ g++ gaussianDenoise.cpp -o gaussianDenoise
$ g++ sobel.cpp -o sobel
$ ./colorToGray HW2_images/Tiger.raw HW2_images/tigerGray.raw
$ ./gaussianDenoise HW2_images/tigerGray.raw HW2_images/tigerDenoise.raw 1 481 321 3
$ ./sobel HW2_images/tigerGray.raw HW2_images/sobelTiger.raw 1 481 321 3

Problem 1.b)
OSX: $ g++ -g -Wall -I/opt/local/include/ canny.cpp -o canny
ubuntu: g++ -g -Wall -I/usr/local/include/opencv4 canny.cpp -o canny

Problem 2.a.1)
$ g++ randomHalfTone.cpp -o randomHalfTone
$ ./randomHalfTone HW2_images/bridge.raw HW2_images/bridgeRandom.raw 1 600 400

Problem 2.a.2)
$ g++ dithering.cpp -o dithering
$ ./dithering HW2_images/bridge.raw HW2_images/bridgeI2.raw 1 600 400 2
$ ./dithering HW2_images/bridge.raw HW2_images/bridgeI4.raw 1 600 400 4
$ ./dithering HW2_images/bridge.raw HW2_images/bridgeI8.raw 1 600 400 8

Problem 2.b.1)
$ g++ errorDiffusion.cpp -o errorDiffusion
$ ./errorDiffusion HW2_images/bridge.raw HW2_images/bridge-FS.raw 1 600 400 1
$ ./errorDiffusion HW2_images/bridge.raw HW2_images/bridge-JJN.raw 1 600 400 2
$ ./errorDiffusion HW2_images/bridge.raw HW2_images/bridge-STU.raw 1 600 400 3
