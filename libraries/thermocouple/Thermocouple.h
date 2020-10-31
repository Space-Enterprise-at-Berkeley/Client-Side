/*
  therm.h - A c++ library to interface with the DS18B20 thermocouple
  Created by Vamshi Balanaga, Sept 18, 2020.
*/

#ifndef __THERMS__
#define __THERMS__

#include <OneWire.h>


using namespace std;

namespace Thermocouple {

  int OW_DATA_PIN;

  byte sensorData[12];
  byte rom[8];

  uint8_t numThermos;
  int currSensorNum = 0;
  float celsius;
  OneWire _ow;

  void init(int numSensors, int owPinNum = 11){
    OW_DATA_PIN = owPinNum;
    _ow = OneWire(OW_DATA_PIN);
    numThermos = numSensors;
    _ow.reset();
    delay(250);
    _ow.search(rom);
  }

  void scrollToRightSensor(int whichSensor){
    bool searched = false;
    if(whichSensor < currSensorNum){
       _ow.reset_search();
       currSensorNum = 0;
       delay(250);
       searched = true;
    }
    for (int i = currSensorNum; i <= whichSensor; i++) {
      _ow.search(rom);
      currSensorNum = i;
      searched=true;
    }
    if(searched) {
      _ow.reset();
      _ow.select(rom);
      _ow.write(0x44, 1);

      delay (400);

      _ow.reset();
      _ow.select(rom);
      _ow.write(0xBE);
    }
    // for(int i = 0; i < 8; i++){
    //   Serial.println(rom[i]);
    // }
  }

  /**
   * Alias for 'scrollToRightSensor'. Makes more sense to user.
  */
  void setSensor(int whichSensor){
    if(whichSensor > numThermos){
      whichSensor = 0;
    }
    scrollToRightSensor(whichSensor);
  }


  void readTemperatureData(float *data) {
    for (int i = 0; i < 8; i++) { // need 9 bytes apparently.
      sensorData[i] = _ow.read();
    }
    int16_t raw = (sensorData[1] << 8) | sensorData[0];
    celsius = (float)raw / 16.0;
    //Serial.println(celsius);
    data[0] = celsius;
    data[1] = -1;
    // Serial.print("celsius: ");
    // Serial.println(celsius);
  }

};
#endif
