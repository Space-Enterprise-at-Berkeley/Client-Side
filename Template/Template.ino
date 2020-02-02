/*
  Template.ino - This is the brain of SEB rockets.
  Created by Shao-Qian Mah, Jan 25, 2020.
*/

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
#define MY_ID 1
// Change this to the polling rate as mentioned in the datasheet. This is in HERTZ.
// Set to 0 to maximize based on the loop speed, this would be useful for clients that only accept commands.
#define POLLING_RATE_HZ 0

// ~~~~~~~~~~~~~~~~~~ Packet Config ~~~~~~~~~~~~~~~~~~
#define STARTCHAR '('
#define ENDCHAR ')'

long lastUpdate = millis();

void setup() {
  // Setup serials
  INTERNALSERIAL.begin(RS485BAUD);

  // Allow control of RS485 pins.
  pinMode(RSCONTROL, OUTPUT);
  digitalWrite(RSCONTROL, LOW);
}

void loop() {
  String data = "THIS SOME PLACEHOLDER DATA! Get real data based on the current sensor";
  long currentTime = millis();
  if ((currentTime - lastUpdate) >= (1000 / POLLING_RATE_HZ)) {
    lastUpdate = currentTime;
    // Do something to update the data here.
  }
  //Listen for signals over the RS485.
  while (INTERNALSERIAL.available() > 0) {
    String packet = readPacket(MY_ID, millis(), MAXCHARS, TIMEOUTMILLIS);
    if (packet.length() != 0) {
      // Valid packet received, send data.
      String transmitPacket = "("+ idToString(MY_ID) + data + ")";
      internalTransmit(&transmitPacket);
    }
  }
}
