/*
  ducer.h - A c++ library to interface with the analog pressure transducers via the
  Created by Vamshi Balanaga, Sept 18, 2020.
*/

#ifndef __DUCERS__
#define __DUCERS__

#include <Arduino.h>
#include <Wire.h>
#include "ADS1219.h"
// #include <TwoWire.h>

using namespace std;

namespace Ducers {

  int DTRDY_PIN_1 = 14;
  int DTRDY_PIN_2 = 28;
  int RESET_PIN_1 = 15;
  int RESET_PIN_2 = 29;

  // first 7 bits for Arduino Wire
  int ADC1_ADDR = 0b1001010;
  int ADC2_ADDR = 0b1001000;

  ADS1219 ads1(DTRDY_PIN_1, ADC1_ADDR);
  ADS1219 ads2(DTRDY_PIN_2, ADC2_ADDR);

  // first 3 bytes are mux select, gain, data rate, data rate, conversion mode, v ref
  int ADC_SETTINGS  = 0b00001111;

  int MUX_SEL_0 = 0b01100000;
  int MUX_SEL_1 = 0b10000000;
  int MUX_SEL_2 = 0b10100000;
  int MUX_SEL_3 = 0b11000000;
  int MUX_SEL_CALIBRATE = 0b11100000;

  int RDATA_COMMAND = 0b00010000;

  int READ_REG_0 = 0b00100000;
  int READ_REG_1 = 0b00100100;

  int WREG_CONF = 0b01000000;

  int WR_START_SYNC = 0b00001000;

  TwoWire *localWire;

  uint8_t buffer[4];

  uint32_t calibration1 = 0;
  uint32_t calibration2 = 0;
  uint32_t calibration1_data[10];

  int resetADC(int ADDR) {
    localWire->beginTransmission(ADDR);
    localWire->write(0b00000110);
    localWire->endTransmission();
    return 0;
  }

  uint32_t readData(int ADDR){
    localWire->beginTransmission(ADDR);
    localWire->write(RDATA_COMMAND);
    localWire->endTransmission();

    localWire->requestFrom(ADDR, 3);

    buffer[0] = localWire->read();
    buffer[1] = localWire->read();
    buffer[2] = localWire->read();

    return (buffer[0] << 16) + (buffer[1] << 8) + buffer[2];
  }

  uint32_t calibrateADC(int ADDR) {
    localWire->beginTransmission(ADDR);
    localWire->write(REG_0_ADDR); // write to reg 0
    localWire->write(MUX_SEL_CALIBRATE | ADC_SETTINGS);
    localWire->endTransmission();
    delay(50);

    uint64_t calibrate = 0;
    for (int i = 0; i <  10; i++){
      uint32_t temp = readData(ADDR);
      calibrate += temp;
    }

    return uint32_t (calibrate / 10);
  }

  bool isDataReady(int ADDR){
    // localWire->beginTransmission(ADDR);
    // localWire->write(READ_REG_1);
    // localWire->endTransmission();
    //
    // localWire->requestFrom(ADDR, 1);
    // int dataReady = localWire->read();
    // Serial.println(dataReady);

    if(ADDR == ADC1_ADDR) {

    } else if (ADDR == ADC2_ADDR) {

    }

    return dataReady & 0b10000000;
  }

  void init() {
    // resetADC(ADC1_ADDR);
    // resetADC(ADC2_ADDR);

    ads1.begin();
    ads2.begin();

    ads1.setConversionMode(CONTINUOUS);
    ads2.setConversionMode(CONTINUOUS);

    ads1.start();
    ads2.start();

    ads1.setVoltageReference(REF_EXTERNAL);
    ads2.setVoltageReference(REF_EXTERNAL);

    ads1.setGain(GAIN_ONE);
    ads2.setGain(GAIN_ONE);

    ads1.setDataRate(20);
    ads2.setDataRate(20);

    // calibration1 = calibrateADC(ADC1_ADDR);
    // calibration2 = calibrateADC(ADC2_ADDR);



    // delay(100);
    //
    // localWire->beginTransmission(ADC1_ADDR);
    // localWire->write(WREG_CONF); // write to reg 0
    // localWire->write(ADC_SETTINGS);
    // localWire->endTransmission();
    //
    // localWire->beginTransmission(ADC2_ADDR);
    // localWire->write(WREG_CONF); // write to reg 0
    // localWire->write(MUX_SEL_0 | ADC_SETTINGS); // always the same high pt input pin on the second ADC
    // localWire->endTransmission();
    //
    // delay(50);
    //
    // localWire->beginTransmission(ADC1_ADDR);
    // localWire->write(WR_START_SYNC); // write start sync
    // localWire->endTransmission();
    //
    // localWire->beginTransmission(ADC2_ADDR);
    // localWire->write(WR_START_SYNC); // write start sync
    // localWire->endTransmission();
    //
    // delay(50);
  }

  void readPropaneTankPressure(float *data) {
    //AIN0
    if(isDataReady(ADC1_ADDR)){
      localWire->beginTransmission(ADC1_ADDR);
      localWire->write(WR_START_SYNC);
      localWire->write(WREG_CONF); // write to reg 0
      localWire->write(MUX_SEL_0 | ADC_SETTINGS);
      localWire->endTransmission();
      delay(35); // test without this.

      uint32_t temp = readData(ADC1_ADDR);
      // Serial.println(temp);
      // Serial.println(temp - calibration1);
      // Serial.println(float(temp-calibration1));

      data[0] = float(temp - calibration1);
    } else {
      data[0] = -1;
    }

    data[1] = -1;
  }

  void readLOXTankPressure(float *data) {
    if (isDataReady(ADC1_ADDR)){
      //AIN1
      localWire->beginTransmission(ADC1_ADDR);
      localWire->write(WR_START_SYNC);
      localWire->write(WREG_CONF); // write to reg 0
      localWire->write(MUX_SEL_1 | ADC_SETTINGS);
      localWire->endTransmission();
      delay(35); // test without this

      data[0] = float(readData(ADC1_ADDR) - calibration1);
    } else {
      data[0] = -1;
    }
    data[1] = -1;
  }

  void readPropaneInjectorPressure(float *data) {
    //AIN2
    localWire->beginTransmission(ADC1_ADDR);
    localWire->write(WR_START_SYNC);
    localWire->write(WREG_CONF); // write to reg 0
    localWire->write(MUX_SEL_2 | ADC_SETTINGS);
    localWire->endTransmission();
    delay(35); // test without this

    data[0] = float(readData(ADC1_ADDR) - calibration1);
    data[1] = -1;
  }

  void readLOXInjectorPressure(float *data) {
    //AIN3
    localWire->beginTransmission(ADC1_ADDR);
    localWire->write(WR_START_SYNC);
    localWire->write(WREG_CONF); // write to reg 0
    localWire->write(MUX_SEL_3 | ADC_SETTINGS);
    localWire->endTransmission();
    delay(35); // test without this

    data[0] = float(readData(ADC1_ADDR) - calibration1);
    data[1] = -1;
  }

  void readHighPressure(float *data) {
    //AIN 0 on ADC 2
    localWire->beginTransmission(ADC2_ADDR);
    localWire->write(WR_START_SYNC); // write start sync
    localWire->endTransmission();

    data[0] = float(readData(ADC2_ADDR) - calibration2);
    data[1] = -1;
  }

}

#endif
