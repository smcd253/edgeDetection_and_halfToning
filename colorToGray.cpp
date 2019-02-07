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
	
	// Check if width is specified
	if (argc < 4) width = 256;
	else width = atoi(argv[3]);

	// check if height is specified
	if (argc < 5) height = 256;
	else height = atoi(argv[4]);
	
	
	// Allocate source image data array
	unsigned char sourceImageData[height][width][3];
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


	// convert to grayscale
    if (BytesPerPixel > 1)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                destImageData[i][j][0] = (unsigned char)(
                    0.25f * (float)sourceImageData[i][j][0] + 
                    0.5f * (float)sourceImageData[i][j][1] + 
                    0.25f * (float)sourceImageData[i][j][2]
                );
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
