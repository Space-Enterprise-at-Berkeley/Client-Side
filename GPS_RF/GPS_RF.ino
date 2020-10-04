// Test code for Ultimate GPS Using Hardware Serial (e.g. GPS Flora or FeatherWing)
//
// This code shows how to listen to the GPS module via polling. Best used with
// Feathers or Flora where you have hardware Serial and no interrupt
//
// Tested and works great with the Adafruit GPS FeatherWing
// ------> https://www.adafruit.com/products/3133
// or Flora GPS
// ------> https://www.adafruit.com/products/1059
// but also works with the shield, breakout
// ------> https://www.adafruit.com/products/1272
// ------> https://www.adafruit.com/products/746

#include <Wire.h>
#include <GPS.h>

int lol = 0;

#define GPSSerial Serial2
//SoftwareSerial GPSSerial(2, 3);
GPS gps(&GPSSerial);


uint32_t timer = millis();


void setup()
{
  GPSSerial.begin(115200);
  //while (!Serial);  // uncomment to have the sketch wait until Serial is ready
  //Wire.begin();

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(9600);
  Serial.println("Adafruit GPS library basic test!");

  gps.init();

  delay(1000);

}

float positionData[3];
float auxilliaryData[5];
void loop() // run over and over again
{
  
  //if (gps.gotSatelliteFix()){
    gps.readAuxilliaryData(auxilliaryData);
    Serial.print("num sats: ");
    Serial.println((int)auxilliaryData[3]);
    
    if(gps.dataAvailable()){

      gps.readPositionData(positionData);
      Serial.print("lat, long: ");
      Serial.print(positionData[0]);
      Serial.print(", ");
      Serial.println(positionData[1]);
    } else {
      Serial.println("NO data");
    }
//  } else {
//    Serial.println("No fix");
//  }
  delay(1000);
}
