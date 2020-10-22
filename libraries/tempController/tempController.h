/*
  tempControl.cpp - A c++ algorithm that maintains the temperature at a given set point.
  Two versions: One very simple, naive algorithm. (Needs to be empirically tested to verify).
  Another, very complex control loop that should reject all disturbances quite well.
  Created by Vamshi Balanaga, Oct 7, 2020.
*/

#ifndef __TEMP_CONTROLLER__
#define __TEMP_CONTROLLER__
#include <cmath>


using namespace std;

namespace tempController {


  int algorithmChoice; // 1 - naive, 2 - actual control theory
  int setPointTemp;
  float k_p = 100;
  float k_i = 10;
  float k_d = 10;


  int init(int tempSetPoint, int _algorithmChoice) {
    if (_algorithmChoice > 2 || _algorithmChoice < 0) {
      return -1;
    }
    algorithmChoice = _algorithmChoice;
    setPointTemp = tempSetPoint;
    return 0;
  }

  int controlTemp(int currTemp) {
    int voltageOut = 0;
    if (algorithmChoice == 1) { // naive
      voltageOut = (int)(k_p * (setPointTemp - currTemp));
      if(voltageOut > 0){
        return 1; // this threshold is dependent on the values of k_p, k_i, k_d;
      } else {
        return 0;
      }
    } else if (algorithmChoice == 2) { // linear control theory solution
      voltageOut = (int)(k_p * (setPointTemp - currTemp));
      voltageOut = max(0, min(255, voltageOut));
      return voltageOut;
    }
  }
};
#endif
