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

float histogram(unsigned char ***sourceImageData, int height, int width, int i, int j, int N)
{
	float numerator = (float)*(((unsigned char *)sourceImageData + reflectY(height, i, nh)*width) + reflectX(width, j, nw));;
	return numerator;
}

int main(int argc, char *argv[])
{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int width, height;
	int N;
	
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

	// check if N for filtering is specified
	if (argc < 7) N = 3;
	else N = atoi(argv[6]);
	
	
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
    // build gaussiand pdf and cdf to normalize histogram
    double gaussian_pdf[256] = {0.0};
	double gaussian_cdf[256] = {0.0};
	for(int i=0; i<256; i++)
		gaussian_pdf[i] = (1/sqrt(800*PI)) * exp(-(((i-70)*(i-70))/800.0));

	gaussian_cdf[0] = gaussian_pdf[0];
	for(int i=1; i<256; i++)
	{
		gaussian_cdf[i] = gaussian_cdf[i-1] + gaussian_pdf[i];
	}

    // build sourceImageData histogram
	int histogram[256] = {0};
	for(int i=0; i < height; i++)
		for(int j=0; j < width; j++)
		{
			histogram[sourceImageData[i][j][0]]++;
		}

	for(int i=1; i<256; i++)
	{
		histogram[i] = histogram[i-1] + histogram[i];
	}

	for(int i=0; i < height; i++)
		for(int j=0; j < width; j++)
			for(int k=0; k<255; k++)
			{
				if((histogram[sourceImageData[i][j][0]]>=(gaussian_cdf[k]*height*width))&&(histogram[sourceImageData[i][j][0]]<(gaussian_cdf[k+1]*height*width)))
					destImageData[i][j][0] = k;
				else if(histogram[sourceImageData[i][j][0]]>=(gaussian_cdf[255]*height*width))
					destImageData[i][j][0] = sourceImageData[i][j][0];
			}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			destImageData[i][j][0] = (unsigned char)histogram((unsigned char ***)sourceImageData, height, width, i, j, N);
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
