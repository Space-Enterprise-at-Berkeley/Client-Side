/*
  GPS.cpp - A c++ library to interface with the Arduino Ultimate GPS MTK3339.
  Intended to be used in I2C mode.
  Created by Vamshi Balanaga, Aug 21, 2020.
*/
#include "GPS.h"
// ======================================NEED TO TEST ==========================

GPSECHO false

void GPS::init(HardwareSerial *ser) {
  Adafruit_GPS _gps(ser);
  commMethod = 1;
  init();
}

void GPS::init(TwoWire *theWire) {
  Adafruit_GPS _gps(theWire);
  commMethod = 3;
  init();
}

void GPS::init(SPIClass *theSPI, int8_t cspin) {
  Adafruit_GPS _gps(theSPI, cspin);
  commMethod = 4;
  init();
}

void GPS::init() {
  _gps.begin(9600);

  _gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  _gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  _gps.sendCommand(PGCMD_ANTENNA);

  delay(1000);
}

bool GPS::dataAvailable() {
  return _gps.newNMEAreceived();
}

bool GPS::gotSatelliteFix() {
   _gps.read();
   return _gps.fix;
}

void GPS::readPositionData(float *data) {
  _gps.read();
  data[0] = _gps.latitudeDegrees;
  data[1] = _gps.longitudeDegrees;
  data[2] = -1;
}

/**
 * Define this in advance. Need to be agreed on by everyone.
 */
void GPS::readAuxilliaryData(float *data) {
  _gps.read();
  data[0] = _gps.altitude;
  data[1] = _gps.speed;
  data[2] = _gps.angle;
  data[3] = _gps.satellites;
  data[4] = -1;
}
