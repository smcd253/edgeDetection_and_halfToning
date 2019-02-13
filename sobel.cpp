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

float xKernel[3][3] = {{-1.0, 0.0, 1.0},
                    {-2.0, 0.0, 2.0},
                    {-1.0, 0.0, 1.0}};
float yKernel[3][3] = {{1.0, 2.0, 1.0},
                    {0.0, 0.0, 0.0},
                    {-1.0, -2.0, -1.0}};

int reflectY(int height, int i, int nh)
{
	if(i + nh >= 0)
	{
		if (i + nh < height - 1)
			return i + nh;
		else
			return i - nh;
	}
	else
		return i - nh;
}
int reflectX(int width, int j, int nw)
{
	if(j + nw >= 0)
	{
		if (j + nw < width - 1)
			return j + nw;
		else
			return j - nw;
	}
	else
		return j - nw;
}

float xDiff(unsigned char ***sourceImageData, int height, int width, int i, int j, int N)
{
	float sum = 0;
	for (int nh = -(N-1)/2; nh <= (N-1)/2; nh++)
	{
		for (int nw = -(N-1)/2; nw <= (N-1)/2; nw++)
		{
			sum += ((float)*(((unsigned char *)sourceImageData + reflectY(height, i, nh)*width) + reflectX(width, j, nw))) * xKernel[nh][nw];
		}
	}
	return sum;
}
float yDiff(unsigned char ***sourceImageData, int height, int width, int i, int j, int N)
{
	float sum = 0;
	for (int nh = -(N-1)/2; nh <= (N-1)/2; nh++)
	{
		for (int nw = -(N-1)/2; nw <= (N-1)/2; nw++)
		{
			sum += ((float)*(((unsigned char *)sourceImageData + reflectY(height, i, nh)*width) + reflectX(width, j, nw))) * yKernel[nh][nw];
		}
	}
	return sum;
}

float sobel(unsigned char ***sourceImageData, int height, int width, int i, int j, int N)
{
    float yGrad = yDiff((unsigned char ***)sourceImageData, height, width, i, j, N);
    float xGrad = xDiff((unsigned char ***)sourceImageData, height, width, i, j, N);

    return sqrtf(xGrad * xGrad + yGrad * yGrad);
}

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int width, height;
	int N = 3; // sobel mask size
    float perc;
    int buf;
	
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

    if (argc < 7) perc = 0.5;
    else perc = atof(argv[6]);

	// Allocate source image data array
	unsigned char sourceImageData[height][width][BytesPerPixel];
    // temp float array for normalization
    float preNormalized[height][width];
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

    // process image

	// build sourceImageData histogram
	int histogram[256] = {0};
	for(int i=0; i < height; i++)
	{
		for(int j=0; j < width; j++)
		{
			histogram[sourceImageData[i][j][0]]++;
		}
	}
	// build histogram cdf
	for (int i = 0; i < 256; i++)
	{
		histogram[i] = histogram[i-1] + histogram[i];
		//printf("histogram[%d] = %d\n", i, histogram[i]);
	}
	// find threshold based on %
	int range = histogram[255] - histogram[0];
	int h = 0;
	int th = 0;
	while(histogram[h] < perc*range)
	{
		th = h;
		h++;
	}
	//printf("th = %d\n", th);
    float max = 0.0;
    float min = 0.0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float pVal = sobel((unsigned char ***)sourceImageData, height, width, i, j, N);
            if(pVal > max)
                max = pVal;
            if(pVal < min)
                min = pVal;
            preNormalized[i][j] = pVal;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float normalized = ((preNormalized[i][j]/(max - min))*255.0);
            //if ((normalized < th + buf) && (normalized > th - buf)) destImageData[i][j][0] = (unsigned char)255;
            if (normalized > th) destImageData[i][j][0] = (unsigned char)0;
			else destImageData[i][j][0] = (unsigned char)255;
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
