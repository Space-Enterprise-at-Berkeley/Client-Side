/*
  tempControl.cpp - A c++ algorithm that maintains the temperature at a given set point.
  Two versions: One very simple, naive algorithm. (Needs to be empirically tested to verify).
  Another, very complex control loop that should reject all disturbances quite well.
  Created by Vamshi Balanaga, Oct 7, 2020.
*/


using namespace std;

class tempController {

private:
  int algorithmChoice; // 1 - naive, 2 - actual control theory
  int setPointTemp;
  float k_p = 1;
  float k_i = 10;
  float k_d = 10;

public:
  void init(int tempSetPoint, int algorithmChoice);
  bool controlTemp(int currTemp);

};
