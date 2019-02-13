#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

int main(int argc, char *argv[])
{
	FILE *file;
	int BytesPerPixel = 1;
	int Length = 0;
	int Width = 0;
	
	if (argc < 4)
	{
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

	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Length*Width*BytesPerPixel, file);
	fclose(file);

	unsigned char ImageTrans1[Width][Length][BytesPerPixel];  //I2
	unsigned char ImageTrans2[Width][Length][BytesPerPixel];  //I8
	unsigned char ImageTrans3[Width][Length][BytesPerPixel];  //I4
	unsigned char ImageTrans4[Width][Length][BytesPerPixel];  //A4
	unsigned char ImageTrans5[Width][Length][BytesPerPixel];  //I2_4

//I2=[0,2;3,1]
//T2=[31.875,159.375;223.125,95.625]
	unsigned char T2[2][2] = {31,159,223,95};
	for(int i=0; i<Width; i++)
		for(int j=0; j<Length; j++)
			for(int k=0; k<BytesPerPixel; k++)
			{
				if(Imagedata[i][j][k]>T2[i%2][j%2])
					ImageTrans1[i][j][k] = 255;
				else
					ImageTrans1[i][j][k] = 0;
			}

//I8=[0    32     8    40     2    34    10    42
//   48    16    56    24    50    18    58    26
//   12    44     4    36    14    46     6    38
//   60    28    52    20    62    30    54    22
//    3    35    11    43     1    33     9    41
//   51    19    59    27    49    17    57    25
//   15    47     7    39    13    45     5    37
//   63    31    55    23    61    29    53    21]
//T8=[1.9922  129.4922   33.8672  161.3672    9.9609  137.4609   41.8359  169.3359
//  193.2422   65.7422  225.1172   97.6172  201.2109   73.7109  233.0859  105.5859
//   49.8047  177.3047   17.9297  145.4297   57.7734  185.2734   25.8984  153.3984
//  241.0547  113.5547  209.1797   81.6797  249.0234  121.5234  217.1484   89.6484
//   13.9453  141.4453   45.8203  173.3203    5.9766  133.4766   37.8516  165.3516
//  205.1953   77.6953  237.0703  109.5703  197.2266   69.7266  229.1016  101.6016
//   61.7578  189.2578   29.8828  157.3828   53.7891  181.2891   21.9141  149.4141
//  253.0078  125.5078  221.1328   93.6328  245.0391  117.5391  213.1641   85.6641]
	unsigned char T8[8][8] = {1,129,33,161,9,137,41,169,193,65,225,97,201,73,233,105,49,177,17,145,57,185,25,153,241,113,209,81,249,121,217,89,13,141,45,173,5,133,37,165,205,77,237,109,197,69,229,101,61,189,29,157,53,181,21,149,253,125,221,93,245,117,213,85};
	for(int i=0; i<Width; i++)
		for(int j=0; j<Length; j++)
			for(int k=0; k<BytesPerPixel; k++)
			{
				if(Imagedata[i][j][k]>T8[i%8][j%8])
					ImageTrans2[i][j][k] = 255;
				else
					ImageTrans2[i][j][k] = 0;
		}

//I4=[0,8,2,10;12,4,14,6;3,11,1,9;15,7,13,5]
//T4=[7.9688  135.4688   39.8438  167.3438
//  199.2188   71.7188  231.0938  103.5938
//   55.7812  183.2812   23.9062  151.4062
//  247.0312  119.5312  215.1562   87.6562]
	unsigned char T4[4][4] = {7,135,39,167,199,71,231,103,55,183,23,151,247,119,215,87};
	for(int i=0; i<Width; i++)
		for(int j=0; j<Length; j++)
			for(int k=0; k<BytesPerPixel; k++)
			{
				if(Imagedata[i][j][k]>T4[i%4][j%4])
					ImageTrans3[i][j][k] = 255;
				else
					ImageTrans3[i][j][k] = 0;
			}

//A4=[14,10,11,15;9,3,0,4;8,2,1,5;13,7,6,12]
//TA4=[231.0938  167.3438  183.2812  247.0312
//     151.4062   55.7812    7.9688   71.7188
//     135.4688   39.8438   23.9062   87.6562
//     215.1562  119.5312  103.5938  199.2188]
	unsigned char TA4[4][4] = {231,167,183,247,151,55,7,71,135,39,23,87,215,119,103,199};
	for(int i=0; i<Width; i++)
		for(int j=0; j<Length; j++)
			for(int k=0; k<BytesPerPixel; k++)
			{
				if(Imagedata[i][j][k]>TA4[i%4][j%4])
					ImageTrans4[i][j][k] = 255;
				else
					ImageTrans4[i][j][k] = 0;
			}

//I2_4=[0,2;3,1]
//T2_4=[31.875,159.375;223.125,95.625]
	unsigned char T2_4[2][2] = {31,159,223,95};
	for(int i=0; i<Width; i++)
		for(int j=0; j<Length; j++)
			for(int k=0; k<BytesPerPixel; k++)
			{
				if(Imagedata[i][j][k]>T2[i%2][j%2])
					ImageTrans5[i][j][k] = 255;
				else if(Imagedata[i][j][k]>T2[i%2][j%2]*2/3)
					ImageTrans5[i][j][k] = 170;
				else if(Imagedata[i][j][k]>T2[i%2][j%2]/3)
					ImageTrans5[i][j][k] = 85;
				else
					ImageTrans5[i][j][k] = 0;
			}		

	if (!(file=fopen("man_I2.raw","wb"))) {
		cout << "Cannot open file: " << "man_I2.raw" << endl;
		exit(1);
	}
	fwrite(ImageTrans1, sizeof(unsigned char), Length*Width*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen("man_I8.raw","wb"))) {
		cout << "Cannot open file: " << "man_I8.raw" << endl;
		exit(1);
	}
	fwrite(ImageTrans2, sizeof(unsigned char), Length*Width*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen("man_I4.raw","wb"))) {
		cout << "Cannot open file: " << "man_I4.raw" << endl;
		exit(1);
	}
	fwrite(ImageTrans3, sizeof(unsigned char), Length*Width*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen("man_A4.raw","wb"))) {
		cout << "Cannot open file: " << "man_A4.raw" << endl;
		exit(1);
	}
	fwrite(ImageTrans4, sizeof(unsigned char), Length*Width*BytesPerPixel, file);
	fclose(file);

	if (!(file=fopen("man_I2_4.raw","wb"))) {
		cout << "Cannot open file: " << "man_I2_4.raw" << endl;
		exit(1);
	}
	fwrite(ImageTrans5, sizeof(unsigned char), Length*Width*BytesPerPixel, file);
	fclose(file);

	return 0;
}