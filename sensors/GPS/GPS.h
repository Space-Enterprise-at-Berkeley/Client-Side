/*
  GPS.h - A c++ library to interface with the Arduino Ultimate GPS MTK3339.
  Created by Vamshi Balanaga, Aug 21, 2020.
*/

#include <Adafruit_GPS.h>
#include "TimerInterrupt.h"

using namespace std{


class GPS {
  public:
    GPS(HardwareSerial *ser); // Constructor when using Serial
    GPS(TwoWire *theWire);    // Constructor when using I2C
    GPS(SPIClass *theSPI, int8_t cspin); // Constructor when using SPI
    void read_position_data(float *data);
    void read_auxilliary_data(float *data);
    bool got_satellite_fix();
    bool data_available();
    bool start_read_interrupt(TimerInterrupt *timer);
  private:
    uint8_t commMethod; // 1 for HardwareSerial, 2 for SoftwareSerial, 3 for I2c, 4 for SPI
    AdaFruit_GPS _gps;
    void init();
}
