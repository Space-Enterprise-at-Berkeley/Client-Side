
#include <GPS.h>

#define GPSECHO false

GPS::GPS(HardwareSerial *ser){
  AdaFruit_GPS _gps(ser);
  init();
}

GPS::GPS(TwoWire *theWire){
  AdaFruit_GPS _gps(theWire);
  init();
}

GPS::GPS(SPIClass *theSPI, int8_t cspin){
  Adafruit_GPS _gps(theSPI, cspin);
  init();
}

void init(){
  _gps.begin(9600);

  _gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  _gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  _gps.sendCommand(PGCMD_ANTENNA);
}

int GPS::read_data(){
  _gps.read();
}
