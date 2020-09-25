/*
  therm.h - A c++ library to interface with the DS18B20 thermocouple
  Created by Vamshi Balanaga, Sept 18, 2020.
*/
#include <OneWire.h>

#define OW_DATA_PIN 2

using namespace std;

class Thermocouple {
  public:
    Thermocouple(int num);
    void readTemperatureData(float *data, int whichSensor);
    void init();
    byte sensorData[12];
    byte rom[8];

  private:
    uint8_t numThermos;
    int currSensorNum = 0;
    float celsius;
    OneWire _ow;
    void scrollToRightSensor(int whichSensor);
};
