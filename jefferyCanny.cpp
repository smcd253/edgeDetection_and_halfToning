/*************************************************************************
 > EE569 Homework Assignment #3
 > Date:     March 24, 2017
 > Author:   Chenyu Peng
 > ID:       3498-8893-91
 > email:    chenyupe@usc.edu
 
 > Compiled on OS X with Xcode
 
 ************************************************************************/

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>


using namespace cv;
using namespace std;


uchar *Imagedata = new uchar[481 *321 * 3];

Mat src, src_gray;

FILE *file;
int Length = 481;
int Width = 321;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio1 = 3;
int kernel_size = 3;
char* window_name = "Edge Map";


Mat dst, detected_edges;


void CannyThreshold(int, void*)
{
	blur(src_gray, detected_edges, Size(3, 3));

	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio1, kernel_size);

	dst = Scalar::all(0);

	src_gray.copyTo(dst, detected_edges);
	imshow(window_name, dst);
}



int main(int argc, char** argv)
{
/*	if (!(file = fopen("Boat.raw", "rb"))) {
		cout << "Cannot open file: " << "Boat.raw" << endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Length*Width*3, file);
	fclose(file);*/
    
    if (!(file = fopen("Castle.raw", "rb"))) {
        cout << "Cannot open file: " << "Castle.raw" << endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Length*Width*3, file);
    fclose(file);
    
    
	src=cv::Mat(Size(Length,Width), CV_8UC3, Imagedata);
	//imshow("edge", src_gray);
	

    if (!src.data)
    {
        return -1;
    }


    dst.create(src_gray.size(), src.type());

    cvtColor(src, src_gray, CV_BGR2GRAY);

    namedWindow(window_name, CV_WINDOW_AUTOSIZE);

    createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

    CannyThreshold(0, 0);

    waitKey(0);

    return 0;
}