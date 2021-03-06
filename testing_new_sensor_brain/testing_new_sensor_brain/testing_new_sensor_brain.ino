/*
  SD.ino - SD interface for Client side microcontroller. 
  Created by Vamshi Balanaga, Feb 24, 2020..
*/

#include <SD.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "Adafruit_BMP3XX.h"

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 6
#define CHIP_SELECT 10

// ~~~~~~~~~~~~~~~~~~ Packet Config ~~~~~~~~~~~~~~~~~~
#define STARTCHAR '('
#define ENDCHAR ')'

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BMP3XX bmp; // I2C
//Adafruit_BMP3XX bmp(BMP_CS); // hardware SPI
//Adafruit_BMP3XX bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

#define SENSOR_INPUT_PIN 8

#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);

File file;

long lastUpdate = millis();
bool atApogee(int baroReading);
String response = "";
float alt;
float xacc;
float yacc;
float zacc;

void setup() {
  // Setup serials
  Serial.begin(115200);
  Serial.println("Accelerometer data access test"); Serial.println("");

  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);

  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
  }
  else {
    Serial.println("IMU initialised");
  }

  if (!SD.begin(CHIP_SELECT)){
    Serial.println("Card failed, or not present.");
  }
  else {
  Serial.println("Card initialised");
  }
  
  if(!bmp.begin()){
    Serial.println("Could not find a valid BMP388 sensor, check wiring!");
  }
  else {
    Serial.println("Barometer initialised");
  }

    bno.setExtCrystalUse(true);
    
  file = SD.open("lad4.txt", O_CREAT | O_WRITE);

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  SPI.begin();
}

void loop() { 

  if (! bmp.performReading()) {
    Serial.println("Barometer Failed to perform reading :(");
    alt = 0;
  }
  else {
    alt = bmp.readAltitude(SEALEVELPRESSURE_HPA);
  }
  sensors_event_t event;
  if (! bno.getEvent(&event)) {
    Serial.println("IMU Failed to perform reading :/");
    xacc = 0;
    yacc = 0;
    zacc = 0;
  }
  else {
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    xacc = euler.x();
    yacc = euler.y();
    zacc = euler.z();
  }
  
  file = SD.open("lad4.txt", O_CREAT | O_WRITE);

  String packet = String(alt) + " , " + String(xacc) + " , " + String(yacc) + " , " + String(zacc);
  file.println(packet);
  Serial.println(packet);

  file.flush();
  file.close();

}
