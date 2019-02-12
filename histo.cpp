#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

#define PI 3.14159265

using namespace std;

int main(int argc, char *argv[])

{
	FILE *file;
	int BytesPerPixel = 3;
	int Length = 0;
	int Width = 0;
	
	if (argc < 4){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw Length Width" << endl;
		return 0;
	}
	
	if (argc >= 4)
	{
		Length = atoi(argv[2]);
		Width = atoi(argv[3]);
	}
	
	unsigned char Imagedata[Width][Length][BytesPerPixel];
	unsigned char Imagetrans[Width][Length][BytesPerPixel];

	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Length*Width*BytesPerPixel, file);
	fclose(file);

	double histogram_gaussian_pdf[256] = {0.0};
	double histogram_gaussian_cdf[256] = {0.0};
	for(int i=0; i<256; i++)
		histogram_gaussian_pdf[i] = (1/sqrt(800*PI)) * exp(-(((i-70)*(i-70))/800.0));

	histogram_gaussian_cdf[0] = histogram_gaussian_pdf[0];
	for(int i=1; i<256; i++)
	{
		histogram_gaussian_cdf[i] = histogram_gaussian_cdf[i-1] + histogram_gaussian_pdf[i];
	}

	int histogram[256] = {0};
	for(int i=0; i<Width; i++)
		for(int j=0; j<Length; j++)
		{
			histogram[Imagedata[i][j][0]]++;
		}

	for(int i=1; i<256; i++)
	{
		histogram[i] = histogram[i-1] + histogram[i];
	}

	for(int i=0; i<Width; i++)
		for(int j=0; j<Length; j++)
			for(int k=0; k<255; k++)
			{
				if((histogram[Imagedata[i][j][0]]>=(histogram_gaussian_cdf[k]*Length*Width))&&(histogram[Imagedata[i][j][0]]<(histogram_gaussian_cdf[k+1]*Length*Width)))
					Imagetrans[i][j][0] = k;
				else if(histogram[Imagedata[i][j][0]]>=(histogram_gaussian_cdf[255]*Length*Width))
					Imagetrans[i][j][0] = Imagedata[i][j][0];
			}

	if (!strcmp(argv[1],"forest_1.raw"))
	{
		if (!(file=fopen("forest_1_trans.raw","wb"))) {
			cout << "Cannot open file: " << "forest_1_trans.raw" << endl;
			exit(1);
		}
		fwrite(Imagetrans, sizeof(unsigned char), Length*Width*BytesPerPixel, file);
		fclose(file);
	}

	if (!strcmp(argv[1],"forest_2.raw"))
	{
		if (!(file=fopen("forest_2_trans.raw","wb"))) {
			cout << "Cannot open file: " << "forest_2_trans.raw" << endl;
			exit(1);
		}
		fwrite(Imagetrans, sizeof(unsigned char), Length*Width*BytesPerPixel, file);
		fclose(file);
	}

	return 0;
}