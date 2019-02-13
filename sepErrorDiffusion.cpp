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

// index matrices
float FS[3][3] =	{{0,0,0},
                    {0,0,7},
                    {3,5,1}};


void diffuseErrorRight(int ***errorMatrix, int height, int width, int i, int j, int c, int N, int sub)
{
	for (int nh = -(N-1)/2; nh <= (N-1)/2; nh++)
	{
		for (int nw = -(N-1)/2; nw <= (N-1)/2; nw++)
		{
			*((((int *)errorMatrix + c*height*width) + (i+nh)*width) + (j + nw)) += (FS[nh][nw]/16.0) * (*((((int *)errorMatrix + c*height*width) + i*width) + j) - sub);
        }    
	}
    if(i == 0)
        printf("(%d,%d,%d)\n", i, j, c);
}
void diffuseErrorLeft(int ***errorMatrix, int height, int width, int i, int j, int c, int N, int sub)
{
	for (int nh = -(N-1)/2; nh <= (N-1)/2; nh++)
	{
		for (int nw = -(N-1)/2; nw <= (N-1)/2; nw++)
		{
        	*((((int *)errorMatrix + c*height*width) + (i+nh)*width) + (j + nw)) += (FS[nh][(N-1)/2 - nw]/16.0) * (*((((int *)errorMatrix + c*height*width) + i*width) + j) - sub);
        }    
	}
}

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int width, height;
	int select;
    int th = 127;
	
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
	
	
    // int array to hold error diffusion data (2 rows and columns bigger to accomodate diffusion at edges)
    int errDiff[height + 2][width + 2][BytesPerPixel];
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
    int N = 3;

    // compare pixels to threshold matrix and half-tone accordingly
	for (int i = 0; i < height; i++)
	{
        if(i%2) // snake right
        {
            for (int j = 0; j < width; j++)
            {
                for (int c = 0; c < BytesPerPixel; c++)
                {
                    if(sourceImageData[i][j][c] + errDiff[i][j][c] > th)
                    {
                        // diffuseErrorRight((int ***)errDiff, height, width, i, j, c, N, 255);
                        for (int nh = -(N-1)/2; nh <= (N-1)/2; nh++)
                        {
                            for (int nw = -(N-1)/2; nw <= (N-1)/2; nw++)
                            {
                                errDiff[i + nh][j + nw][c] += (int)(FS[nh][nw]/16.0 * ((float)errDiff[i][j][c] - 255.0));
                            }    
                        }
                        destImageData[i][j][c] = 255;   
                    }
                    else
                    {
                        //diffuseErrorRight((int ***)errDiff, height, width, i, j, c, N, 0);
                        for (int nh = -(N-1)/2; nh <= (N-1)/2; nh++)
                        {
                            for (int nw = -(N-1)/2; nw <= (N-1)/2; nw++)
                            {
                                errDiff[i + nh][j + nw][c] += (int)(FS[nh][nw]/16.0 * (float)errDiff[i][j][c]);
                            }    
                        }
                        destImageData[i][j][c] = 0;   
                    }
                } 
            }
        }
        else // snake left
        {
            for (int j = width - 1; j >= 0; j--)
            {
                for (int c = 0; c < BytesPerPixel; c++)
                {
                    if(sourceImageData[i][j][c] + errDiff[i][j][c] > th)
                    {
                        // diffuseErrorLeft((int ***)errDiff, height, width, i, j, c, N, 255);
                        for (int nh = -(N-1)/2; nh <= (N-1)/2; nh++)
                        {
                            for (int nw = -(N-1)/2; nw <= (N-1)/2; nw++)
                            {
                                errDiff[i + nh][j + nw][c] += (int)(FS[nh][(N-1)/2 - nw]/16.0 * ((float)errDiff[i][j][c] - 255.0));
                            }    
                        }
                        destImageData[i][j][c] = 255;   
                    }
                    else
                    {
                        //diffuseErrorLeft((int ***)errDiff, height, width, i, j, c, N, 0);
                        for (int nh = -(N-1)/2; nh <= (N-1)/2; nh++)
                        {
                            for (int nw = -(N-1)/2; nw <= (N-1)/2; nw++)
                            {
                                errDiff[i + nh][j + nw][c] += (int)(FS[nh][(N-1)/2 - nw]/16.0 * (float)errDiff[i][j][c]);
                            }    
                        }
                        destImageData[i][j][c] = 0;   
                    }
                } 
            }
        }
	}

	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) 
	{
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}

	fwrite(destImageData, sizeof(unsigned char), height*width*BytesPerPixel, file);
	fclose(file);

	return 0;
}
