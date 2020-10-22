/*
 * Template_I2C.ino - A c++ program that uses I2C communication to receive requests
 * for data from Brain_I2C.ino and send the corresponding data back.
 * Created by Vainavi Viswanath, Aug 21, 2020.
 */
#include <Wire.h>
//#include <ducer.h>
/*
 * Change this address according to the board address this code is uploaded on
 */
int Address = 8;

#define LOX_TANK_PIN A0
#define PROP_TANK_PIN A1
#define LOX_INJECTOR_PIN A2
#define PROP_INJECTOR_PIN A3
#define HIGH_PIN A4

int lox_tank_pressure, prop_tank_pressure, lox_injector_pressure, prop_injector_pressure, high_pressure;


int sensorNum = 0;
void (*generalFunc)(float*);

/*
 * Data structure to allow the conversion of bytes to floats and vice versa.
 */
union floatArrToBytes {
  char buffer[24];
  float sensorReadings[6];
} farrbconvert;



void setup() {
  Wire.begin(Address);    
  Wire.onRequest(requestEvent); 
  Wire.onReceive(setSensorNum);
  Serial.begin(9600);

  pinMode(LOX_TANK_PIN, INPUT);
  pinMode(PROP_TANK_PIN, INPUT);
  pinMode(LOX_INJECTOR_PIN, INPUT);
  pinMode(PROP_INJECTOR_PIN, INPUT);
  pinMode(HIGH_PIN, INPUT);

  lox_tank_pressure = 0;
  prop_tank_pressure = 0;
  lox_injector_pressure = 0;
  prop_injector_pressure = 0;
  high_pressure = 210; // to prevent interpolation from fucking up; reading is from 1-5V, which is ~200 - 1024

  farrbconvert.sensorReadings[0] = -1;
  farrbconvert.sensorReadings[1] = -1;
  farrbconvert.sensorReadings[2] = -1;
  farrbconvert.sensorReadings[3] = -1;
  farrbconvert.sensorReadings[4] = -1;
  farrbconvert.sensorReadings[5] = -1;

  Serial.println("finished setup");
}

void loop() {
  delay(100);
}


/*
 * When a request comes from the brain, collects data from the requested sensor
 * and sends back float array with data points. 
 */
void requestEvent() {
  Wire.write(farrbconvert.buffer, 24);
}

/*
 * Selects the corresponding function to the sensor which data is requested from.
 */
void setSensorNum(int howMany) {
  while (!Wire.available());
  sensorNum = Wire.read();
  Serial.println(sensorNum);
  switch (sensorNum) {
    case 1:
      lox_injector_pressure = analogRead(LOX_INJECTOR_PIN);
      farrbconvert.sensorReadings[0] = lox_injector_pressure;
//      Ducers::readLOXInjectorPressure(farrbconvert.sensorReadings);
      break;
    case 2:
      prop_injector_pressure = analogRead(PROP_INJECTOR_PIN);
      farrbconvert.sensorReadings[0] = prop_injector_pressure;
//      Ducers::readPropaneInjectorPressure(farrbconvert.sensorReadings);
      break;
    case 3:
      lox_tank_pressure = analogRead(LOX_TANK_PIN);
      farrbconvert.sensorReadings[0] = lox_tank_pressure;
//      Ducers::readLOXTankPressure(farrbconvert.sensorReadings);
      break;
    case 4:
      prop_tank_pressure = analogRead(PROP_TANK_PIN);
      farrbconvert.sensorReadings[0] = prop_tank_pressure;

//      Ducers::readPropaneTankPressure(farrbconvert.sensorReadings);
      break;
    case 5:
      high_pressure = analogRead(HIGH_PIN);
      farrbconvert.sensorReadings[0] = high_pressure;

//      Ducers::readHighPressure(farrbconvert.sensorReadings);
      break;
     default:
      break;
  }
  Serial.println(farrbconvert.sensorReadings[0]);
}
