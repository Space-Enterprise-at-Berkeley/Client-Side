#include <loadCell.h>
#include "brain_utils.h"

#define THIS_ADDR 1

const int numSensors = 3;

sensorInfo sensors[numSensors] = {
  // local sensors
  {"Temperature",                THIS_ADDR, 3, 1}, //&(testTempRead)}, //&(Thermocouple::readTemperatureData)},
  {"Load Cells",                 THIS_ADDR, 4, 1},
};

float data[6];

#define BRAINSERIAL Serial2

void setup() {
  // put your setup code here, to run once:
  LoadCell::init();
  BRAINSERIAL.begin(9600);
   Serial.begin(9600);
    while (!Serial) {
      delay(10);
    }
    Serial.println("initialized");

//    /* Initialise the driver with I2C_ADDRESS and the default I2C bus. */
//    if (! mcp.begin(I2C_ADDRESS)) {
//        Serial.println("Sensor not found. Check wiring!");
//        while (1);
//    }
//
//  Serial.println("Found MCP9600!");
//
//  mcp.setADCresolution(MCP9600_ADCRESOLUTION_18);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (BRAINSERIAL.available()) {
    String s_id = BRAINSERIAL.readStringUntil('\n');
    Serial.println(s_id);
    int id = s_id.toInt();
    getData(id, data);
    String packet = make_packet(id);
    Serial.println(packet);
    BRAINSERIAL.println(packet);
  }
  
  for (int i = 0; i < 6; i++){
    Serial.print(data[i]);
    Serial.print(", ");
  }
  Serial.println("");

}

void getData(int id, float *data){
  Serial.print("id ");
  Serial.println(id);
  switch(id){
    case 3: // load cell
      //LoadCell::readLoad(data);
      data[0] = 0;
      data[1] = 1;
      data[2] = -1;
      break;
    case 4: //thermocouples
      data[0] = -1423; //cryo
      data[1] = 203; //ow 1
      data[1] = 9234; //ow 2
      data[2] = 293; // ow 3
      data[3] = -1;
  }
}
