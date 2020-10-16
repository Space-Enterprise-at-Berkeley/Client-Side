/*
  therm.h - A c++ library to interface with the DS18B20 thermocouple
  Created by Vamshi Balanaga, Sept 18, 2020.
*/

#ifndef __DUCERS__
#define __DUCERS__

#include <OneWire.h>

#define OW_DATA_PIN 2

using namespace std;

namespace Thermocouple {

  byte sensorData[12];
  byte rom[8];

  uint8_t numThermos;
  int currSensorNum = 0;
  float celsius;
  OneWire _ow;

  void init(int numSensors){
    _ow = OneWire(OW_DATA_PIN);
    numThermos = numSensors;
    _ow.reset();
    delay(250);
    _ow.search(rom);
  }

  /**
   * whichSensor is 0 indexed.
   */
  void readTemperatureData(float *data, int whichSensor) {
    if(whichSensor >= numThermos) {
      data[0] = -1;
      return;
    }

    scrollToRightSensor(whichSensor);

    for (int i = 0; i < 9; i++) { // need 9 bytes apparently.
      sensorData[i] = _ow.read();
    }

    int16_t raw = (sensorData[1] << 8) | sensorData[0];
    celsius = (float)raw / 16.0;
    data[0] = celsius;
    data[1] = -1;
  }

  void scrollToRightSensor(int whichSensor){
    if(whichSensor < currSensorNum){
       _ow.reset_search();
       currSensorNum = 0;
       delay(250);
    }
    for (int i = currSensorNum; i < whichSensor; i++){
      _ow.search(rom);
      currSensorNum = i;
    }
    _ow.reset();
    _ow.select(rom);
    _ow.write(0x44, 1);

    delay (1000);

    _ow.reset();
    _ow.select(rom);
    _ow.write(0xBE);
  }
};
#endif
