#include <Wire.h>

int Address = 8;

void setup() {
  Wire.begin(Address);           // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  union floatToBytes {
    char buffer[4];
    float voltageReading;
  } converter;
  
  converter.voltageReading = getReading0(); //sensor reading here
  Wire.write(converter.buffer, 4);
}

float getReading0() { //more methods (getReading1, getReading2, etc for other sensorIDs)
  //get the sensor reading here
  return 36.89;
}
