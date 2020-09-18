/*
  GPS.ino - GPS Client side microcontroller code.
  Created by Vamshi Balanaga, Feb 25, 2020.
*/

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

// ~~~~~~~~~~~~~~~~~~ Serial Config - Arduino Pro Mini ~~~~~~~~~~~~~~~~~~
#define GPSSerial Serial

SoftwareSerial INTERNALSERIAL(5, 6);
#define RS485BAUD 9600

// ~~~~~~~~~~~~~~~~~~ RS485 Config ~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
#define MY_ID 5
// Change this to the polling rate as mentioned in the datasheet. This is in HERTZ.
// Set to 0 to maximize based on the loop speed, this would be useful for clients that only accept commands.
#define POLLING_RATE_HZ 1

// ~~~~~~~~~~~~~~~~~~ Packet Config ~~~~~~~~~~~~~~~~~~
#define STARTCHAR '('
#define ENDCHAR ')'

Adafruit_GPS GPS(&GPSSerial);

long lastUpdate = millis();
String response = "";

void setup() {
  // Setup serials
  INTERNALSERIAL.begin(RS485BAUD);

  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY); // type of data
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);

  // Allow control of RS485 pins.
  pinMode(RSCONTROL, OUTPUT);
  digitalWrite(RSCONTROL, LOW);
}

void loop() {
  //Listen for signals over the RS485.
  while (INTERNALSERIAL.available() > 0) {
    String packet = readPacket(MY_ID, millis(), MAXCHARS, TIMEOUTMILLIS);
    if (packet.length() != 0) {
      // Valid packet received, send data.
      response = "lat:"+String(GPS.latitude) + GPS.lat + ",lon:" + String(GPS.longitude) + GPS.lon + ",alt:" + String(GPS.altitude);
      String transmitPacket = "("+ idToString(MY_ID) + response + ")";
      internalTransmit(&transmitPacket);
    }
  }
