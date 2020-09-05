#include <Wire.h>

int Address = 8;
int sensorNum = 0;

void setup() {
  Wire.begin(Address);           // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(updateSensorNum);
  Serial.begin(9600);
}

void loop() {
  delay(100);
}

union floatArrToBytes {
  char buffer[24];
  float sensorReadings[6];
} farrbconvert;

void requestEvent() {
  float data[6] = {0,0,0,0,0,0};
  if (sensorNum == 0) {
    getReading0(data);
  } else if (sensorNum == 1) {
    getReading1(data);
  }
  for (int i=0; i<6; i++) {
    farrbconvert.sensorReadings[i] = data[i];
  }
  Wire.write(farrbconvert.buffer, 24);
}

void updateSensorNum(int howMany) {
  while (!Wire.available());
  sensorNum = Wire.read();
}

void getReading0(float data[6]) {
  //get the sensor reading here
  data[0] = 36.89;
  data[1] = 25.50;
  data[2] = 30.45;
}

float getReading1(float data[6]) {
  //get the sensor reading here
  data[0] = 14.26;
}
