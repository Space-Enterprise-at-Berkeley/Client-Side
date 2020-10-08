/*
  tempControl.cpp - A c++ algorithm that maintains the temperature at a given set point.
  Two versions: One very simple, naive algorithm. (Needs to be empirically tested to verify).
  Another, very complex control loop that should reject all disturbances quite well.
  Not sure how this will work since our control of the heater is simply on or off.
  Created by Vamshi Balanaga, Oct 7, 2020.
*/

#include "tempController.h"
#include <cmath>


void tempController::init(int tempSetPoint, int _algorithmChoice) {
  if (_algorithmChoice > 2 || _algorithmChoice < 0) {
    exit(1);
  }
  algorithmChoice = _algorithmChoice;
  setPointTemp = tempSetPoint;
}

bool tempController::controlTemp(int currTemp) {
  int voltageOut = 0;
  if (algorithmChoice == 1) { // naive
    voltageOut = (int)(k_p * (setPointTemp - currTemp));
  } else if (algorithmChoice == 2) { // linear control theory solution

  }
  return voltageOut > 10; // this threshold is dependent on the values of k_p, k_i, k_d;
}
