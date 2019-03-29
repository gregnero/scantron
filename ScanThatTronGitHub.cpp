#include <ctime>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  bool verbose = false;
  string src_filename = "";
  string dst_filename = "";

  po::options_description options("Options");
  options.add_options()("help,h", "display this message")(
      "verbose,v", po::bool_switch(&verbose), "verbose [default is silent]")(
      "source-filename,i", po::value<string>(&src_filename), "source filename");

  po::positional_options_description positional_options;
  positional_options.add("source-filename", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
                .options(options)
                .positional(positional_options)
                .run(),
            vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << "Usage: " << argv[0] << " [options] source-filename" << endl;
    cout << options << endl;
    return EXIT_SUCCESS;
  }

  if (!boost::filesystem::exists(src_filename)) {
    cerr << "Provided source file does not exists" << endl;
    return EXIT_FAILURE;
  }

  cv::Mat src = cv::imread(src_filename, cv::IMREAD_UNCHANGED);

  if (verbose) {
    cout << "Source filename: " << src_filename << endl;
    cout << "Size: " << src.size() << endl;
    cout << "Channels: " << src.channels() << endl;
  }
  
  clock_t startTime = clock();

  /**************************************** Main Code and what not *********************************/

  // Convert to grayscale if color
  cv::Mat gray = src;
  if(src.channels() > 1){
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
  }
  
  // Add a Gaussian Blur to gray image
  // General 3 by 3 gaussian blur filter used (Maybe look into increasing kernel size?)(3 by 3 is pretty subtle)
  cv::Mat gaussFilter;
  cv::Mat gauss;

  gaussFilter.create(3, 3, CV_32FC1);
  gaussFilter.at<float>(0, 0) = 0.0113;
  gaussFilter.at<float>(1, 0) = 0.0838;
  gaussFilter.at<float>(2, 0) = 0.0113;

  gaussFilter.at<float>(0, 1) = 0.0838;
  gaussFilter.at<float>(1, 1) = 0.6193;
  gaussFilter.at<float>(2, 1) = 0.0838;

  gaussFilter.at<float>(0, 2) = 0.0113;
  gaussFilter.at<float>(1, 2) = 0.0838;
  gaussFilter.at<float>(2, 2) = 0.0113;

  cv::filter2D(gray, gauss, CV_32FC1, gaussFilter); //Convolve gray image with gaussian filter
  
  // Apply adaptive thresholding (NOTE: The blank scan tron sheet is technically already binary!)
  cv::Mat binary;
  gauss.convertTo(gauss,src.type()); 
  
  cv::adaptiveThreshold(gauss, binary, 255, cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY,7, 7); //3rd parameter scales binary mat to 0s and 255s
  
  /*************************************************************************************************/
  
  clock_t endTime = clock();

  if (verbose) {
    cout << "Elapsed time: "
         << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
         << " [s]" << endl;
  }
  
  cv::namedWindow("src",cv::WINDOW_NORMAL); 
  cv::resizeWindow("src", 800,800); //Cause this image is fooking huuugggee
  cv::imshow("src",binary);
  cv::waitKey(0);


  return EXIT_SUCCESS;
}
