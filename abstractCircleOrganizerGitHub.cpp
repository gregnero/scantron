#include "abstractCircleOrganizer.h"

#include <iostream>

using namespace std;

namespace ipcv {

bool circleOrganizer(const vector<cv::Point> &bubbleXY, vector<vector<cv::Point>> &LastNames,
                                                               vector<vector<cv::Point>> &FirstNames,
                                                               vector<vector<cv::Point>> &UniversityID,
                                                               vector<vector<cv::Point>> &AdditionalInfo,
                                                               vector<vector<cv::Point>> &col1,
                                                               vector<vector<cv::Point>> &col2,
                                                               vector<vector<cv::Point>> &col3,
                                                               vector<vector<cv::Point>> &col4,
                                                               vector<vector<cv::Point>> &col5,
                                                               vector<vector<cv::Point>> &col6) {
                                                                                                                                                                                         
/*************** Place bubble circles into categories *******************/
  //Consider attempting this in 1 loop maybe?
  //Did in multiple loops for easy visualization because this is pretty abstract

  //Last Name bubble locations
  int N = 0;
  int C = 11;
  int gap = 0;
  for (int i = 0; i < bubbleXY.size(); i++) {  
  
    if (i == C) {
      gap++;
      if ((gap >= 11 && gap < 15) || (gap > 24)) {
        i += (18 - 9);
        C += ((18 + 11) - 9);
        N++;
      } else {
        i += 18;
        C += 18 + 11;
        N++;
      }
    }      
    if (N == 26) {
      break;
    }
    LastNames[N].push_back(bubbleXY[i]);
  }
  
  //First Name bubble locations
  N = 0;
  C = 20;
  gap = 0;
  for(int i = 11; i < bubbleXY.size(); i++){
    if (i == C) {
      gap++;
      if ((gap >= 11 && gap < 15) || (gap > 24)) {
        i += (20 - 9);
        C += ((20 + 9) - 9);
        N++;
      } else {
        i += 20;
        C += 20 + 9;
        N++;
      }
    }       
    if (N == 26) {
      break;
    }
    FirstNames[N].push_back(bubbleXY[i]);        
  }

  //University ID bubble locations
  N = 0;
  C = 29;
  for(int i = 20; i < bubbleXY.size(); i++){
    if (i == C) {
        i += 20;
        C += 20 + 9;
        N++;     
    }      
    if (N == 10) {
      break;
    }
    UniversityID[N].push_back(bubbleXY[i]);
  }
  
  //Additional Information bubble locations
  N = 0;
  C = (29 * 15) - (9 * 4);
  for(int i = C - 9; i < bubbleXY.size(); i++){
    if (i == C) {
        i += 20;
        C += 20 + 9;
        N++;     
    }   
    
    if (N == 10) {
      break;
    }
    AdditionalInfo[N].push_back(bubbleXY[i]);
  }

  //Answers bubble locations  
  int sConst = (LastNames.size() * LastNames[0].size()) + (FirstNames.size() * FirstNames[0].size())
         + (UniversityID.size() * UniversityID[0].size()) + (AdditionalInfo.size() * AdditionalInfo[0].size());  
         
  int s = (LastNames.size() * LastNames[0].size()) + (FirstNames.size() * FirstNames[0].size())
         + (UniversityID.size() * UniversityID[0].size()) + (AdditionalInfo.size() * AdditionalInfo[0].size());  
  N = 0;
  C = s + 5;  
  for(s; s < bubbleXY.size(); s++){   
      if (s == C) {
        N++;
        s += 25;
        C += 30;     
      } 
      if (N == 25) {
        break;
      }   
      col1[N].push_back(bubbleXY[s]);                               
  }
  
  s = sConst + 5;
  N = 0;
  C = s + 5; 
  for(s; s < bubbleXY.size(); s++){ 
    if (s == C) {
        N++;
        s += 25;
        C += 30;     
      } 
      if (N == 25) {
        break;
      }   
      col2[N].push_back(bubbleXY[s]);                               
  }
  
  s = sConst + 10;
  N = 0;
  C = s + 5;
  for(s; s < bubbleXY.size(); s++){ 
    if (s == C) {
        N++;
        s += 25;
        C += 30;     
      } 
      if (N == 25) {
        break;
      }   
      col3[N].push_back(bubbleXY[s]);                               
  }
  
  s = sConst + 15;
  N = 0;
  C = s + 5;
  for(s; s < bubbleXY.size(); s++){ 
    if (s == C) {
        N++;
        s += 25;
        C += 30;     
      } 
      if (N == 25) {
        break;
      }   
      col4[N].push_back(bubbleXY[s]);                               
  }
  
  s = sConst + 20;
  N = 0;
  C = s + 5;
  for(s; s < bubbleXY.size(); s++){ 
    if (s == C) {
        N++;
        s += 25;
        C += 30;     
      } 
      if (N == 25) {
        break;
      }   
      col5[N].push_back(bubbleXY[s]);                               
  }
  
  s = sConst + 25;
  N = 0;
  C = s + 5;
  for(s; s < bubbleXY.size(); s++){ 
    if (s == C) {
        N++;
        s += 25;
        C += 30;     
      } 
      if (N == 25) {
        break;
      }   
      col6[N].push_back(bubbleXY[s]);                               
  }                                                               


  return true;
}
}