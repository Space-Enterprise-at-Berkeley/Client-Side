/*
  Barometer.cpp - A c++ library to interface with the Arduino BMP3XX Barometer.
  Created by Vamshi Balanaga, Sept 15, 2020.
*/

#include "Barometer.h"

// ==================== NEED TO TEST =======================================

Barometer::Barometer(TwoWire *theWire){
  _bmp = Adafruit_BMP3XX(-1);
  init(theWire);
}

void Barometer::init(TwoWire *theWire) {
  if(!_bmp.begin(BMP3XX_DEFAULT_ADDRESS, theWire)){
    exit(1);
  }
  _bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_4X);
  _bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);
  _bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  _bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void Barometer::readAltitude(float *data) {
  data[0] = _bmp.readAltitude(seaLevelPressure);
  data[1] = -1;
}

void Barometer::readPressure(float *data) {
  data[0] = _bmp.readPressure();
  data[1] = -1;
}

void Barometer::readTemperature(float *data) {
  data[0] = _bmp.readTemperature();
  data[1] = -1;
}

bool Barometer::updateSeaLevelPressure(float newPressure) {
  seaLevelPressure = newPressure;
  updatedSeaLevelPressure = true;
  return updateSeaLevelPressure;
}

bool Barometer::getSeaLevelStatus(){
  return updatedSeaLevelPressure;
}
