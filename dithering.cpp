// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of width 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different widths and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;
#define PI 3.14159265

// index matrices
float I2[2][2] =	{{0,2},
					{3,1}};
float I4[4][4] =	{{0,8,2,10},
					{12,4,14,6},
					{3,11,1,9},
					{15,7,13,5}};
float I8[8][8] =  	{{0,32,8,40,2,34,10,42},
					{48,16,56,24,50,18,58,26},
					{12,44,4,36,14,46,6,38},
					{60,28,52,20,62,30,54,22},
					{3,35,11,43,1,33,9,41},
					{51,19,59,27,49,17,57,25},
					{15,47,7,39,13,45,5,37},
					{63,31,55,23,61,29,53,21}};

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int width, height;
	int I;
	
	// Check for proper syntax
	if (argc < 3)
	{
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [width = 256] [height = 256] [BytesPerPixelOut = 1]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4) BytesPerPixel = 1; // default is grey image
	else BytesPerPixel = atoi(argv[3]);

	// Check if width is specified
	if (argc < 5) width = 256;
	else width = atoi(argv[4]);

	// check if height is specified
	if (argc < 6) height = 256;
	else height = atoi(argv[5]);

	// check if I for index matrix is specified
	if (argc < 7) I = 2;
	else I = atoi(argv[6]);
	
	
	// Allocate source image data array
	unsigned char sourceImageData[height][width][BytesPerPixel];
	// Allocate dest image data array
	unsigned char destImageData[height][width][1];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) 
	{
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(sourceImageData, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    
	// build threshold matrix
    unsigned char T[8][8] = {0};
    for (int i = 0; i < I; i++)
    {
        for(int j = 0; j < I; j++)
        {
            switch(I)
            {
                case 2:
                    T[i][j] = (int)(((I2[i][j] + 0.5) / (I*I)) * 255.0);
                break;
                case 4:
                    T[i][j] = (int)(((I4[i][j] + 0.5) / (I*I)) * 255.0);
                break;
                case 8:
                    T[i][j] = (int)(((I8[i][j] + 0.5) / (I*I)) * 255.0);
                break;
            }
        }
    }

    // compare pixels to threshold matrix and half-tone accordingly
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
            for(int c = 0; c < BytesPerPixel; c++)
            {
                if(sourceImageData[i][j][c] > T[i % I][j % I]) destImageData[i][j][c] = 255;
                else destImageData[i][j][c] = 0;
            }
		}
	}

	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) 
	{
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}

	fwrite(destImageData, sizeof(unsigned char), height*width, file);
	fclose(file);

	return 0;
}
