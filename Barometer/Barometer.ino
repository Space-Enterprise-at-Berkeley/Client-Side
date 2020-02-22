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

// ~~~~~~~~~~~~~~~~~~ Client Specific Config ~~~~~~~~~~~~~~~~~~
// CHANGE THESE VALUES!!!!
#define MY_ID 2
// Change this to the polling rate as mentioned in the datasheet. This is in HERTZ.
// Set to 0 to maximize based on the loop speed, this would be useful for clients that only accept commands.
#define POLLING_RATE_HZ 400
#define SENSOR_INPUT_PIN 8
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BMP3XX bmp; //I2C

// ~~~~~~~~~~~~~~~~~~ Packet Config ~~~~~~~~~~~~~~~~~~
#define STARTCHAR '('
#define ENDCHAR ')'

long lastUpdate = millis();
bool atApogee(int baroReading);
String response = "";

void setup() {
  // Setup serials
  INTERNALSERIAL.begin(RS485BAUD);
  while(!INTERNALSERIAL);
  
  if(!bmp.begin()){
    String errorMessage = "Could not find a valid BMP# sensor, check wiring!";
    transmitMessage(&errorMessage);
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
  bmp.performReading();
  long currentTime = millis();
  if ((currentTime - lastUpdate) >= (1000 / POLLING_RATE_HZ)) {
    lastUpdate = currentTime;
    // Do something to update the data here.
    response = atApogee(data)? "true": "false";
  }
  //Listen for signals over the RS485.
  while (INTERNALSERIAL.available() > 0) {
    String packet = readPacket(MY_ID, millis(), MAXCHARS, TIMEOUTMILLIS);
    if (packet.length() != 0) {
      // Valid packet received, send data.
      String transmitPacket = "("+ idToString(MY_ID) + response + ")";
      internalTransmit(&transmitPacket);
    }
  }
}

float previousAltitude[3] = {0,0,0};
long previousAltTimes[3];
int altIndex = 0;

//upward is positive.

bool atApogee(int baroReading){
  float currAltitude = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  currAltitude = kalmanFilter(currAltitude);
  previousAltitude[altIndex] = currAltitude;
  previousAltTimes[altIndex] = millis();
  altIndex++;
  altIndex = altIndex % 3;
  //better check than this????
  return (currAltitude < previousAltitude[altIndex] && previousAltitude[altIndex] < previousAltitude[(altIndex+1)%3]);     
}

float deltaT = 0;
float velocity(){
  deltaT = previousAltTimes[altIndex] - previousAltTimes[(altIndex-1)%3];
  return (previousAltitude[altIndex] - previousAltitude[(altIndex-1)%3]) / deltaT;
}

float previous = 0;
float kalmanfilter(float currReading){
  float estimate = previous + velocity()*deltaT - 4.9*deltaT**2
  float resid = currReading - estimate;
  k = abs(resid / (resid + 1));
  float finalEstimate = estimate + k*resid;
  previous = finalEstimate; 
  return finalEstimate;
}