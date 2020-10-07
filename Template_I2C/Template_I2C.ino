/*
 * Template_I2C.ino - A c++ program that uses I2C communication to receive requests
 * for data from Brain_I2C.ino and send the corresponding data back.
 * Created by Vainavi Viswanath, Aug 21, 2020.
 */
#include <Wire.h>

/*
 * Change this address according to the board address this code is uploaded on
 */
int Address = 8;

int sensorNum = 0;
void (*generalFunc)(float*);

void setup() {
  Wire.begin(Address);    
  Wire.onRequest(requestEvent); 
  Wire.onReceive(updateSensorNum);
  Serial.begin(9600);
}

void loop() {
  delay(100);
}

/*
 * Data structure to allow the conversion of bytes to floats and vice versa.
 */
union floatArrToBytes {
  char buffer[24];
  float sensorReadings[6];
} farrbconvert;

/*
 * When a request comes from the brain, collects data from the requested sensor
 * and sends back float array with data points. 
 */
void requestEvent() {
  float data[6] = {0,0,0,0,0,0};
  generalFunc(data);
  for (int i=0; i<6; i++) {
    farrbconvert.sensorReadings[i] = data[i];
  }
  Wire.write(farrbconvert.buffer, 24);
}

/*
 * Selects the corresponding function to the sensor which data is requested from.
 */
void updateSensorNum(int howMany) {
  while (!Wire.available());
  sensorNum = Wire.read();
  if (sensorNum == 0) {
    generalFunc = &getReading0;
  } else if (sensorNum == 1) {
    generalFunc = &getReading1;
  }
}

/*
 * Method to collect data from ______
 */
void getReading0(float *data) {
  //get the sensor reading here
  data[0] = 36.89;
  data[1] = 25.50;
  data[2] = 30.45;
}

/*
 * Method to collect data from ______
 */
float getReading1(float *data) {
  //get the sensor reading here
  data[0] = 14.26;
}
