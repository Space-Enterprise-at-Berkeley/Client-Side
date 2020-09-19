/*
  therm.h - A c++ library to interface with the DS18B20 thermocouple
  Created by Vamshi Balanaga, Sept 18, 2020.
*/


#include <OneWire.h>

using namespace std;

#define OW_DATA_PIN 5

class Thermocouple {
  public:
    Thermocouple(int num);
    void readTemperatureData(float *data, int whichSensor);

  private:
    void scrollToRightSensor(int whichSensor);
    uint8_t numThermos;
    byte sensorData[12];
    byte rom[8];
    int currSensor = 0;
    float celsius;
    OneWire _ow;
    void init();
}
