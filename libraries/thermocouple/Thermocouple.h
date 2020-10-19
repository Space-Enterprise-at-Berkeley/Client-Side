/*
  therm.h - A c++ library to interface with the DS18B20 thermocouple
  Created by Vamshi Balanaga, Sept 18, 2020.
*/

#ifndef __THERMS__
#define __THERMS__

#include <OneWire.h>

#define OW_DATA_PIN 11

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
    // for(int i = 0; i < 8; i++) {
    //   Serial.print(rom[i]);
    // }
    // Serial.println("initialized");
  }

  void scrollToRightSensor(int whichSensor){
    Serial.println("scrolling");
    bool searched = false;
    if(whichSensor < currSensorNum){
       _ow.reset_search();
       currSensorNum = 0;
       delay(250);
    }
    for (int i = currSensorNum; i <= whichSensor; i++){
      // Serial.println("searching");
      _ow.search(rom);
      currSensorNum = i;
      searched=true;
    }
    if(searched){
      // Serial.println("after searching doing rom stuff");
      // for(int i = 0; i < 8; i++) {
      //   Serial.print(rom[i]);
      // }
      // Serial.println("\n");
      _ow.reset();
      _ow.select(rom);
      _ow.write(0x44, 1);

      delay (1000);

      _ow.reset();
      _ow.select(rom);
      _ow.write(0xBE);
    }
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
    // Serial.println("done scrolling");

    for (int i = 0; i < 9; i++) { // need 9 bytes apparently.
      sensorData[i] = _ow.read();
    }

    int16_t raw = (sensorData[1] << 8) | sensorData[0];
    celsius = (float)raw / 16.0;
    data[0] = celsius;
    data[1] = -1;
    // Serial.print("celsius: ");
    // Serial.println(celsius);
  }

};
#endif
