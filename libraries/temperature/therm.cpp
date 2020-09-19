/*
  therm.cpp - A c++ library to interface with the DS18B20 thermocouple
  Created by Vamshi Balanaga, Sept 18, 2020.
*/

#include "therm.h"

Thermocouple::Thermocouple(int numSensors){
  _ow = OneWire(OW_DATA_PIN);
  numThermos = numSensors;
  _ow.reset();
}

/**
 * whichSensor is 0 indexed.
 */
void Thermocouple::readTemperatureData(float *data, int whichSensor) {
  if(whichSensor >= numSensors){
    data[0] = -1;
    return;
  }

  scrollToRightSensor(whichSensor);

  for (int i = 0; i < 9; i++){ // need 9 bytes apparently.
    sensorData[i] = _ow.read();
  }

  int16_t raw = (sensorData[1] << 8) | sensorData[0];
  celsius = (float)raw / 16.0;
  data[0] = celsius;
  data[1] = -1;
}

void Thermocouple::scrollToRightSensor(int whichSensor){
  if(whichSensor < currSensor){
     _ow.reset_search();
     currSensor = 0;
     delay(250);
   }
  for (int i = currSensor; i <= whichSensor; i++){
    _ow.search(rom);
  }
  _ow.reset();
  _ow.select(rom);
  _ow.write(0xBE);
}
