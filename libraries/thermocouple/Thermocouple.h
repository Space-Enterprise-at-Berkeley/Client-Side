/*
  therm.h - A c++ library to interface with the DS18B20 thermocouple
  Created by Vamshi Balanaga, Sept 18, 2020.
*/

#ifndef __THERMS__
#define __THERMS__

#include <OneWire.h>
#include <ADS1219.h>

#include <Adafruit_I2CDevice.h>
#include <Adafruit_I2CRegister.h>
#include "Adafruit_MCP9600.h"
#include <Wire.h>
#include <typeinfo>

using namespace std;

namespace Thermocouple {


  namespace OW { //onewire
    int OW_DATA_PIN;

    byte sensorData[12];
    byte rom[8];

    uint8_t numThermos;
    int currSensorNum = 0;
    float celsius;
    OneWire _ow;

    void init(int numSensors, int owPinNum = 11) {
      OW_DATA_PIN = owPinNum;
      _ow = OneWire(OW_DATA_PIN);
      numThermos = numSensors;
      _ow.reset();
      delay(500);
      _ow.search(rom);
    }

    void scrollToRightSensor(int whichSensor) {
      bool searched = false;
      if(whichSensor <= currSensorNum) {
         _ow.reset_search();
         currSensorNum = 0;
         delay(250);
         searched = true;
      }
      for (int i = currSensorNum; i <= whichSensor; i++) {
        Serial.println("searching");
        _ow.search(rom);
        currSensorNum = i;
        searched=true;
      }
      Serial.println(searched);
      if(searched) {
        _ow.reset();
        _ow.select(rom);
        _ow.write(0x44, 1);

        delay (1000);
        Serial.println("finishded 700ms wait");

        _ow.reset();
        _ow.select(rom);
        _ow.write(0xBE);
      }
      for(int i = 0; i < 8; i++){
        Serial.print(rom[i]);
      }
      Serial.println("");
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

    float readTemperatureData(float *data) {
      for (int i = 0; i < 8; i++) { // need 9 bytes apparently.
        sensorData[i] = _ow.read();
      }
      int16_t raw = (sensorData[1] << 8) | sensorData[0];
      celsius = (float)raw / 16.0;
      //Serial.println(celsius);
      data[0] = celsius;
      data[1] = -1;
      Serial.print("celsius: ");
      Serial.println(celsius);
      return celsius;
    }
  }

  // namespace Analog { //TMP36
  //   // calibration = 25 C at 750mV
  //
  //   int DTRDY_PIN = 28;
  //   int RESET_PIN = 29;
  //
  //   // first 7 bits for Arduino Wire
  //   uint8_t ADC2_ADDR = 0b1001000;
  //
  //   float tempOverVoltageScale = 1 / 0.01;
  //   float voltageOffset = 0.75, tempOffset = 25; // 25 C = 0.450 V
  //
  //   long rawRead;
  //   float voltageRead;
  //   float tempRead;
  //
  //   ADS1219 ads(DTRDY_PIN, ADC2_ADDR, NULL);
  //
  //   void init() {
  //     // ads.begin();
  //
  //     ads.setConversionMode(CONTINUOUS);
  //
  //     ads.setVoltageReference(REF_EXTERNAL);
  //
  //     ads.setGain(GAIN_ONE);
  //
  //     ads.setDataRate(90);
  //   }
  //
  //   void readTemperatureData(float *data) {
  //     rawRead = ads.readData(2); // thermocouple on AIN1 of ADC2
  //     // Serial.println(rawRead);
  //     // Serial.println(ads.readData(2));
  //     // Serial.println(ads.readData(3));
  //     voltageRead = (float) rawRead * (5.0 / pow(2,23));
  //     Serial.println(voltageRead);
  //     tempRead = ((voltageRead - voltageOffset) * tempOverVoltageScale) + tempOffset;
  //     data[0] = tempRead;
  //     data[1] = -1;
  //   }
  // }

  namespace Cryo {

    Adafruit_MCP9600 ** _cryo_amp_boards;
    int * _addrs;
    int _numSensors;

    int init(int numSensors, int * addrs, _themotype * types) { // assume that numSensors is < max Size of packet. Add some error checking here
      _addrs = (int *)malloc(numSensors);
      _cryo_amp_boards = (Adafruit_MCP9600 **)malloc(numSensors * sizeof(Adafruit_MCP9600));

      _numSensors = numSensors;

      for (int i = 0; i < numSensors; i++) {
        _addrs[i] = addrs[i];
        _cryo_amp_boards[i] = new Adafruit_MCP9600();

        if (!_cryo_amp_boards[i]->begin(addrs[i])) {
          Serial.println("Error initializing cryo board at Addr 0x" + String(addrs[i], HEX));
          return -1;
        }

        _cryo_amp_boards[i]->setADCresolution(MCP9600_ADCRESOLUTION_18);
        _cryo_amp_boards[i]->setThermocoupleType(types[i]);
        _cryo_amp_boards[i]->setFilterCoefficient(3);
        _cryo_amp_boards[i]->enable(true);
      }

      return 0;
    }

    void readCryoTemps(float *data) {
      for (int i = 0; i < _numSensors; i++) {
        data[i] = _cryo_amp_boards[i]->readThermocouple();
      }
      data[_numSensors] = -1;
    }

    int freeAllResources() {
        free(_cryo_amp_boards);
        free(_addrs);
        return 0;
    }
  }

}

#endif
