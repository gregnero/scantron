/** For testing Hough Transform for detcting cirlces 
 *
 * \File: hough.cpp
 * \Author: Gregory M. Nero (gmn8357@rit.edu)
 * \Date: 16-Apr-19
 *
 * usage: ./hough [image] [maxradius]
 *   image: image to read in
 *   maxradius: (int) the max radius you want to test for in range [0, maxradius]
*/

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "imgs/ipcv/utils/Utils.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <math.h>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {

  //Read in image and get rows/cols
  cv::Mat src = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  int rows = src.rows;
  int cols = src.cols;

  //Range of radii you want to test [0, radii_range]
  int radii_range = atoi(argv[2]);


  //Vector to store each radial layer ("the accumulator matrix")
  vector<cv::Mat> H;
  for (int l = 0; l < radii_range; l++) {

    cv::Mat mymat(rows, cols, CV_8UC1);
    mymat = 0;

    H.push_back(mymat);


  }

  //resolution of steps??
  //angle overlap value if too small due to int truncation?
  for (int x = 0; x < cols; x++) {
    for (int y = 0; y < rows; y++) {
      for (int r = 0; r < radii_range; r++) {
        for (double angle = 0; angle <= 360; angle = angle + 20) {

	
	  //Get the current radial layer
	  cv::Mat current_layer = H[r];

	  //a and b values corresponding to current radius and angle
          int a = x - (r * cos(angle * (M_PI/180) ));
	  int b = y - (r * sin(angle * (M_PI/180) ));

	  //To prevent core dumps
	  if (a < 0 || a > cols) {continue;}
	  if (b < 0 || b > rows) {continue;}

	  //Get the code value of the source
	  int src_value = src.at<char>(x,y);

	  //See if there is a code value there
	  if (src_value != 0) {

	    //If so, accumulate
	    current_layer.at<char>(a,b) += 10; //arbitrary accumulator value

	  }
	  

	}
      }
    }
  }

  int threshold = 180; //sort-of arbitrary threshold for 'radtest2.png'
  for (int l = 0; l < radii_range; l++) {

    string radius = to_string(l);
    string title = "Radius: " + radius;


    cv::Mat layer = H[l];

    double min, max;
    cv::Point minloc, maxloc;

    cv::minMaxLoc(layer, &min, &max, &minloc, &maxloc);

    if (max == threshold) {

      cout << "Circle of radius " << l << " has been found at location: "
	<< "(" <<  maxloc.x << ", " << maxloc.y << ")" << endl;     

    }


    cv::imshow(title, layer);
    cv::waitKey(0);
    
  }

  return EXIT_SUCCESS;
}

