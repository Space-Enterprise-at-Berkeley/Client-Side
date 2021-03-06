/*
  Barometer.ino
  Created by Vamshi Balanaga, Feb 2, 2020.
*/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

// ~~~~~~~~~~~~~~~~~~ Serial Config - Arduino Pro Mini ~~~~~~~~~~~~~~~~~~
#define INTERNALSERIAL Serial
#define RS485BAUD 57600

// ~~~~~~~~~~~~~~~~~~ RS485 Config ~~~~~~~~~~~~~~~~~~
// Pin number for receive and transmit modes.
#define RSCONTROL 3
// Milliseconds to wait for non-malformed sensor packet before moving on to the next one.
#define TIMEOUTMILLIS 500
// Max number of chars in a message, including '(' and ')' chars.
#define MAXCHARS 100
// Max number of times to retry querying a sensor before giving up.
#define MAXRETRIES 3

// ~~~~~~~~~~~~~~~~~~ BMP388 Config ~~~~~~~~~~~~~~~~~~
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

// ~~~~~~~~~~~~~~~~~~ Client Specific Config ~~~~~~~~~~~~~~~~~~
// CHANGE THESE VALUES!!!!
#define MY_ID 2
// Change this to the polling rate as mentioned in the datasheet. This is in HERTZ.
// Set to 0 to maximize based on the loop speed, this would be useful for clients that only accept commands.
#define POLLING_RATE_HZ 400
#define SENSOR_INPUT_PIN 8
#define SEALEVELPRESSURE_HPA (1013.25)

//Uncomment one of the following three lines to fit chosen protocol+wiring
//Adafruit_BMP3XX bmp; //I2C
Adafruit_BMP3XX bmp(BMP_CS); // hardware SPI
//Adafruit_BMP3XX bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK); //Software SPI

// ~~~~~~~~~~~~~~~~~~ Packet Config ~~~~~~~~~~~~~~~~~~
#define STARTCHAR '('
#define ENDCHAR ')'

// ~~~~~~~~~~~~~~~~~~ Calculation Config ~~~~~~~~~~~~~~~~~~
#define SEALEVELPRESSURE_HPA (1013.25)

long lastUpdate = millis();
bool atApogee(int baroReading);
String response = "";

void setup() {
  // Setup serials
  INTERNALSERIAL.begin(RS485BAUD);
  while (!INTERNALSERIAL);

  while (!Serial);
  Serial.println("BMP388 test");

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP3 sensor, check wiring!");
    while (1);
  }

  // Allow control of RS485 pins.
  pinMode(RSCONTROL, OUTPUT);
  digitalWrite(RSCONTROL, LOW);

  //Configure BMP Sensor.
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
}

void loop() {
  float altitude;
  bmp.performReading();
  long currentTime = millis();
  if ((currentTime - lastUpdate) >= (1000 / POLLING_RATE_HZ)) {
    lastUpdate = currentTime;
<<<<<<< HEAD
    altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    Serial.println(altitude);
    // float filtered_alt = kalmanFilter(altitude);
=======
    float altitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
   // float filtered_alt = kalmanFilter(altitude);
>>>>>>> 70a8a8854745ece26e81b0edfd75ab3825cbd6c9
    // response = atApogee(altitude)? "true": "false";
  }
  //Listen for signals over the RS485.
  while (INTERNALSERIAL.available() > 0) {
    String packet = readPacket(MY_ID, millis(), MAXCHARS, TIMEOUTMILLIS);
    if (packet.length() != 0) {
      // Valid packet received, send data.
<<<<<<< HEAD
      String transmitPacket = "(" + idToString(MY_ID) + String(altitude) + ")";
      Serial.println("Sending data");
=======
      String transmitPacket = "("+ idToString(MY_ID) + String(altitude) + ")";
>>>>>>> 70a8a8854745ece26e81b0edfd75ab3825cbd6c9
      internalTransmit(&transmitPacket);
    }
  }
}
<<<<<<< HEAD
=======

>>>>>>> 70a8a8854745ece26e81b0edfd75ab3825cbd6c9
