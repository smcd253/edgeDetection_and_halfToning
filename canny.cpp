#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char const* window_name = "Edge Map";

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  src.copyTo( dst, detected_edges);
  imshow( window_name, dst );
 }


/** @function main */
int main( int argc, char** argv )
{
  /// Load an image
  src = imread( argv[1], IMREAD_COLOR );

  if( !src.data )
  { 
    printf("no file or unreadable file. exit.\n");
    return -1; 
  }
  // namedWindow( window_name, WINDOW_AUTOSIZE );
  // imshow( window_name, src );
  printf("1\n");
  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );
  printf("2\n");
  /// Convert the image to grayscale
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  printf("3\n");
  /// Create a window
  namedWindow( window_name, WINDOW_AUTOSIZE );
  printf("4\n");

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
  printf("5\n");

  /// Show the image
  CannyThreshold(0, 0);
  printf("6\n");

  /// Wait until user exit program by pressing a key
  waitKey(0);

  return 0;
  }
