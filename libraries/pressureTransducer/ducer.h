
#ifndef __DUCERS__
#define __DUCERS__

#include <Arduino.h>

using namespace std;

namespace Ducers {

  // first 7 bits for Arduino Wire
  #define ADC1_ADDR 0b10000000
  #define ADC2_ADDR 0b10000001

  // first 3 bytes are mux select, gain, data rate, data rate, conversion mode, v ref
  #define ADC_SETTINGS 0b00001111

  #define MUX_SEL_0 0b01100000
  #define MUX_SEL_1 0b10000000
  #define MUX_SEL_2 0b10100000
  #define MUX_SEL_3 0b11000000
  #define MUX_SEL_CALIBRATE 0b11100000

  #define RDATA_COMMAND 0b00010000

  TwoWire *localWire;

  uint8_t buffer[4];

  uint32_t calibration1;
  uint32_t calibration2;


  int resetADC(int ADDR) {
    localWire->beginTransmission(ADDR);
    localWire->write(0b00000110);
    localWire->endTransmission();
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

  uint32_t callibrateADC(int ADDR){
    localWire->beginTransmission(ADDR);
    localWire->write(0b00100000); // write to reg 0
    localWire->write(MUX_SEL_CALIBRATE | ADC_SETTINGS);
    localWire->endTransmission();
    delay(10);

    uint32_t calibrate = 0;
    for (int i = 0; i <  10; i++){
      calibrate += readData(ADDR);
    }

    return uint32_t (calibrate / 10);
  }

  void init(TwoWire *theWire) {
    localWire = theWire;

    resetADC(1);
    resetADC(2);

    calibration1 = callibrateADC(1);
    calibration2 = callibrateADC(2);

    delay(100);

    localWire->beginTransmission(ADC1_ADDR);
    localWire->write(0b00100000); // write to reg 0
    localWire->write(0b00000000 | ADC_SETTINGS);
    localWire->endTransmission();

    localWire->beginTransmission(ADC2_ADDR);
    localWire->write(0b00100000); // write to reg 0
    localWire->write(MUX_SEL_0 | ADC_SETTINGS); // always the same high pt input pin on the second ADC
    localWire->endTransmission();
  }

  void readPropaneTankPressure(float *data) {
    //AIN0
    localWire->beginTransmission(ADC1_ADDR);
    localWire->write(0b00100000); // write to reg 0
    localWire->write(MUX_SEL_0 | ADC_SETTINGS);
    localWire->endTransmission();
    delay(10); // test without this.

    data[0] = float(readData(ADC1_ADDR) - calibration1);
    data[1] = -1;
  }

  void readLOXTankPressure(float *data) {
    //AIN1
    localWire->beginTransmission(ADC1_ADDR);
    localWire->write(0b00100000); // write to reg 0
    localWire->write(MUX_SEL_1 | ADC_SETTINGS);
    localWire->endTransmission();
    delay(10); // test without this

    data[0] = float(readData(ADC1_ADDR) - calibration1);
    data[1] = -1;
  }

  void readPropaneInjectorPressure(float *data) {
    //AIN2
    localWire->beginTransmission(ADC1_ADDR);
    localWire->write(0b00100000); // write to reg 0
    localWire->write(MUX_SEL_2 | ADC_SETTINGS);
    localWire->endTransmission();
    delay(10); // test without this

    data[0] = float(readData(ADC1_ADDR) - calibration1);
    data[1] = -1;
  }

  void readLOXInjectorPressure(float *data) {
    //AIN3
    localWire->beginTransmission(ADC1_ADDR);
    localWire->write(0b00100000); // write to reg 0
    localWire->write(MUX_SEL_3 | ADC_SETTINGS);
    localWire->endTransmission();
    delay(10); // test without this

    data[0] = float(readData(ADC1_ADDR) - calibration1);
    data[1] = -1;
  }

  void readHighPressure(float *data) {
    //AIN 0 on ADC 2
    data[0] = float(readData(ADC2_ADDR) - calibration2);
    data[1] = -1;
  }

}

#endif
