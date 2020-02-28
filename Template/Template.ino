/*
  Template.ino - Template for Client side microcontroller code. 
  Created by SQ Mah, Jan 25, 2020.
*/


#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

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
#define SENSOR_INPUT_PIN 8

// ~~~~~~~~~~~~~~~~~~ IMU Config ~~~~~~~~~~~~~~~~~~
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);


// ~~~~~~~~~~~~~~~~~~ Packet Config ~~~~~~~~~~~~~~~~~~
#define STARTCHAR '('
#define ENDCHAR ')'


long lastUpdate = millis();
String response = "";

void setup() {
  // Setup serials
  INTERNALSERIAL.begin(RS485BAUD);

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

  bno.setExtCrystalUse(true);
  
  // Allow control of RS485 pins.
  pinMode(RSCONTROL, OUTPUT);
  digitalWrite(RSCONTROL, LOW);
  pinMode(SENSOR_INPUT_PIN, INPUT);
}

void loop() {
  imu::Vector<3> data = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  float xdata = data.x();
  float ydata = data.y();
  float zdata = data.z();
  long currentTime = millis();
  if ((currentTime - lastUpdate) >= (1000 / POLLING_RATE_HZ)) {
    lastUpdate = currentTime;
    // Do something to update the data here.
  }
  //Listen for signals over the RS485.
  while (INTERNALSERIAL.available() > 0) {
    String packet = readPacket(MY_ID, millis(), MAXCHARS, TIMEOUTMILLIS);
    Serial.println(packet);
    if (packet.length() != 0) {
      // Valid packet received, send data.
      String transmitPacket = "("+ idToString(MY_ID) + response + ")";
      Serial.println("Packet sent");
      internalTransmit(&transmitPacket);
    }
  }
}
