#pragma once

#include <opencv2/core.hpp>

using namespace std;

namespace ipcv {

/*

--This function is magical. Please do not bother looking at it. It is very abstract and gross. 
--Note that this function is specially designed for the blank scan tron Carl gave us
--A scan tron that differs from his will fail
--If we have time we can look into a way to make it univeral amognst all scantrons, but I think that
would take longer than the time we have to get this project done
--This took many hours as it is but it is litterally 50% of the project lol

-WTF does it do?
 --Organizes the bubbles into categories of 2D vector of Points 
   --For instance, lets look at "FirstNames"
     --"FirstNames" will be of size 26 because there are 26 letters in the alphabet
     --Each letter is stored in a vector
       --Within each of these vectors there is another vector of size 11 which represents the max length of the response

*/

bool circleOrganizer(const vector<cv::Point> &bubbles, vector<vector<cv::Point>> &LastNames,
                                                               vector<vector<cv::Point>> &FirstNames,
                                                               vector<vector<cv::Point>> &UniversityID,
                                                               vector<vector<cv::Point>> &AdditionalInfo,
                                                               vector<vector<cv::Point>> &col1,
                                                               vector<vector<cv::Point>> &col2,
                                                               vector<vector<cv::Point>> &col3,
                                                               vector<vector<cv::Point>> &col4,
                                                               vector<vector<cv::Point>> &col5,
                                                               vector<vector<cv::Point>> &col6);
}