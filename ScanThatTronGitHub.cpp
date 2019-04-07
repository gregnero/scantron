#include <ctime>
#include <iostream>
#include "abstractCircleOrganizer.h"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

namespace po = boost::program_options;

struct myclass {
  bool operator()(cv::Point pt1, cv::Point pt2) {
    return (pt1.x + (1000 * pt1.y)) <
           (pt2.x + (1000 * pt2.y));  // This is going to sort the bubble vector
                                      // from top to bottom and left to right
  }
} myobject;

int main(int argc, char* argv[]) {
  bool verbose = false;
  string blankScanSheet_filename = "";
  string filledOutSheets_filename = "";

  po::options_description options("Options");
  options.add_options()("help,h", "display this message")(
      "verbose,v", po::bool_switch(&verbose), "verbose [default is silent]")(
      "blank-scan-sheet,i", po::value<string>(&blankScanSheet_filename), "Blank Scan Sheet")(
      "filled-out-sheets,f", po::value<string>(&filledOutSheets_filename), "Filled out Sheets");

  po::positional_options_description positional_options;
  positional_options.add("blank-scan-sheet", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
                .options(options)
                .positional(positional_options)
                .run(),
            vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << "Usage: " << argv[0] << " [options] blank-scan-sheet" << endl;
    cout << options << endl;
    return EXIT_SUCCESS;
  }

  if (!boost::filesystem::exists(blankScanSheet_filename)) {
    cerr << "Provided blank scan sheet file does not exist" << endl;
    return EXIT_FAILURE;
  }

  cv::Mat src = cv::imread(blankScanSheet_filename, cv::IMREAD_UNCHANGED);
  cv::Mat src2 = cv::imread(filledOutSheets_filename, cv::IMREAD_UNCHANGED);

  if (verbose) {
    cout << "Blank Scan Sheet Filename: " << blankScanSheet_filename << endl;
    cout << "Size: " << src.size() << endl;
    cout << "Channels: " << src.channels() << endl;
  }

  clock_t startTime = clock();

  /**************************************** Main Code and what not
   * *********************************/

  // Convert to grayscale if color
  cv::Mat gray = src;
  if (src.channels() > 1) {
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
  }

  vector<cv::Vec3f> circles;

  // Apply the Hough Transform to find the circles
  // Randomly tried a bunch of different threshold values and these seemed to
  // work out ok
  cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1, src.rows / 100, 200,
                   50, 0,
                   src.rows / 50);  // src.rows/100, 200, 60, 0, src.rows/50);

  vector<cv::Vec3f> bubbles(1450);  // 1450 because that is the number of
                                    // bubbles in our scantron  Regular Hough
                                    // Circles gives us some circles that we do
                                    // not care about  This vector will hold only
                                    // the bubble circles
  vector<cv::Point> bubbleXY;

  int c = 0;  // Used to fill our bubbles vector in the loop
  for (size_t i = 0; i < circles.size(); i++) {
    cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
    int radius = cvRound(circles[i][2]);

    if (radius != cvRound(circles[20][2])) {  // This is just so we fill our new
                                              // vector properly
      c = c - 1;
    }

    if (radius == cvRound(circles[20][2])) {  // 99.99% of the time this will be the value
                                    // The radius of all the bubble circles will
                                    // be the same  given the blank scan tron
                                    // image is of extremely high resolution  If
                                    // we come across a circle that has the same
                                    // radius as the other  bubble circles lets
                                    // add that to our bubble circle vector

      bubbles[c][0] = cvRound(circles[i][0]);  // columns
      bubbles[c][1] = cvRound(circles[i][1]);  // rows
      bubbles[c][2] = cvRound(circles[i][2]);  // radius

      bubbleXY.push_back(
          cv::Point(cvRound(circles[i][0]), cvRound(circles[i][1])));
    }
    c++;
  }
  //Sort bubbles from left to right and top to bottom
  std::sort(bubbleXY.begin(), bubbleXY.end(), myobject);
  
  //We now need to organize these bubbles into categories so that it will be easy to match them to letters.
  vector<vector<cv::Point>> LastNames(26);
  vector<vector<cv::Point>> FirstNames(26);
  vector<vector<cv::Point>> UniversityID(10);
  vector<vector<cv::Point>> AdditionalInfo(10);
  vector<vector<cv::Point>> col1(25);
  vector<vector<cv::Point>> col2(25);
  vector<vector<cv::Point>> col3(25);
  vector<vector<cv::Point>> col4(25);
  vector<vector<cv::Point>> col5(25);
  vector<vector<cv::Point>> col6(25);

  //Check "abstractCircleOrganizer.h" for explantion of wtf this does
  ipcv::circleOrganizer(bubbleXY, LastNames, FirstNames, UniversityID, AdditionalInfo, col1, col2, col3, col4, col5, col6);
  
  for(int i = 0; i < FirstNames.size(); i++){
     for(int g = 0; g < FirstNames[0].size(); g++){
      int radius = cvRound(bubbles[i][2]);
      // circle center
      circle( src, FirstNames[i][g], 3, cv::Scalar(0,255,0), -1, 8, 0 );
      // circle outline
      circle( src, FirstNames[i][g], radius, cv::Scalar(0,0,255), 3, 8, 0 );
      
      cv::namedWindow("src", cv::WINDOW_NORMAL);
      cv::resizeWindow("src", 800, 800);  // Cause this image is fooking huuugggee
      cv::imshow("src", src);
      cv::waitKey(1);
    }
  }
  
  /*************************************************************************************************/

  clock_t endTime = clock();

  if (verbose) {
    cout << "Elapsed time: "
         << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
         << " [s]" << endl;
  }

  //cv::namedWindow("src", cv::WINDOW_NORMAL);
  //cv::resizeWindow("src", 800, 800);  // Cause this image is fooking huuugggee
  //cv::imshow("src", src);
  //cv::waitKey(0);

  return EXIT_SUCCESS;
}
