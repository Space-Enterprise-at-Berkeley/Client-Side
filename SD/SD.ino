/*
  SD.ino - SD interface for Client side microcontroller. 
  Created by Vamshi Balanaga, Feb 24, 2020..
*/

#include <SD.h>

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
#define MY_ID 4
// Change this to the polling rate as mentioned in the datasheet. This is in HERTZ.
// Set to 0 to maximize based on the loop speed, this would be useful for clients that only accept commands.
#define POLLING_RATE_HZ 0

#define CHIP_SELECT 10

// ~~~~~~~~~~~~~~~~~~ Packet Config ~~~~~~~~~~~~~~~~~~
#define STARTCHAR '('
#define ENDCHAR ')'

File file;

long lastUpdate = millis();

void setup() {
  // Setup serials
  INTERNALSERIAL.begin(RS485BAUD);

  if (!SD.begin(CHIP_SELECT)){
    Serial.println("Card failed, or not present.");
    return;
  }
  Serial.println("Card initialized");

  // Allow control of RS485 pins.
  pinMode(RSCONTROL, OUTPUT);
  digitalWrite(RSCONTROL, LOW);

  file = SD.open("lad4_datalog.txt", O_CREAT | O_WRITE);
}

void loop() {
  //Listen for signals over the RS485.
  while (INTERNALSERIAL.available() > 0) {
    String packet = readPacket(MY_ID, millis(), MAXCHARS, TIMEOUTMILLIS);
    if (packet.length() != 0) {
      // Valid packet received, send data.
      file.println(packet);
    }
    if(millis() % 20){ // flush every 10 ms, with size of string, and hopefully high transmission rate, this should be good  
      file.flush();
    }
    
  }
}
