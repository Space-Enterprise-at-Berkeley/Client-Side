/*
  GPS.h - A c++ library to interface with the Arduino Ultimate GPS MTK3339.
  Created by Vamshi Balanaga, Aug 21, 2020.
*/

#include <Adafruit_GPS.h>

using namespace std{


class GPS {
  public:
    GPS(HardwareSerial *ser); // Constructor when using Serial
    GPS(TwoWire *theWire);    // Constructor when using I2C
    GPS(SPIClass *theSPI, int8_t cspin); // Constructor when using SPI
    read_data();
  private:
    uint8_t commMethod; // 1 for HardwareSerial, 2 for SoftwareSerial, 3 for I2c, 4 for SPI
    AdaFruit_GPS _gps;
}
