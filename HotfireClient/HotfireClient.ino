//#include <loadCell.h>
#include "brain_utils.h"
#include <Thermocouple.h>

#define THIS_ADDR 1

const int numSensors = 3;

sensorInfo sensors[numSensors] = {
  // local sensors
  {"Temperature",                THIS_ADDR, 3, 1}, //&(testTempRead)}, //&(Thermocouple::readTemperatureData)},
  {"Load Cells",                 THIS_ADDR, 4, 1},
};

float data_temp[6];

#define BRAINSERIAL Serial2
#define NUM_OW_THERMO 3
float ow_temps[NUM_OW_THERMO] = {0, 0, 0};
int curr_ow_thermo = 0;

void setup() {
  BRAINSERIAL.begin(4608000);
  Serial.begin(57600);
  while (!Serial) {
    delay(10);
  }
  Serial.println("initialized");
  
  Thermocouple::Cryo::init();
  Thermocouple::OW::init(NUM_OW_THERMO,22);
  //  LoadCell::init();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (BRAINSERIAL.available()) {
    String s_id = BRAINSERIAL.readStringUntil('\n');
    Serial.println(s_id);
    int id = s_id.toInt();
    getData(id, farrbconvert.sensorReadings);
    String packet = make_packet(id);
    BRAINSERIAL.println(packet);
    BRAINSERIAL.flush();
    Serial.println(packet);
  }

  // OW thermo couples not working yet.

//  Thermocouple::OW::setSensor(curr_ow_thermo);
//  ow_temps[curr_ow_thermo] = Thermocouple::OW::readTemperatureData(data_temp);
//
//  curr_ow_thermo ++;
//  curr_ow_thermo %= NUM_OW_THERMO;
//  Serial.println(ow_temps[curr_ow_thermo]);
}

void getData(int id, float *fdata){
  Serial.print("id ");
  Serial.println(id);
  switch(id){
    case 3: // load cell
      //LoadCell::readLoad(data);
      fdata[0] = 0;
      fdata[1] = 1;
      fdata[2] = -1;
      break;
    case 4: //thermocouples
      Thermocouple::Cryo::readCryoTemp(fdata); //cryo
      fdata[1] = ow_temps[0]; //ow 1
      fdata[2] = ow_temps[1]; //ow 2
      fdata[3] = ow_temps[2]; // ow 3
      fdata[4] = -1;
  }
}
