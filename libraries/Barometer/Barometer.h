/*
  Barometer.h - A c++ library to interface with the Arduino BMP3XX Barometer.
  Created by Vamshi Balanaga, Sept 15, 2020.
*/


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP3xx.h>

using namespace std;

#define BNO055_SAMPLERATE_DELAY_MS (100)


class Barometer {
  public:
    Barometer(TwoWire *theWire);
    void readAltitudeData(float *data);
    void readPressureData(float *data);
    void readTemperatureData(float *data);
    bool updateSeaLevelPressure(float newPressure);
    bool getSeaLevelStatus();

  private:
    Adafruit_BMP3XX _bmp;
    float seaLevelPressure = SEALEVELPRESSURE_HPA;
    bool updatedSeaLevelPressure = false;
    void init(TwoWire *theWire);
};
