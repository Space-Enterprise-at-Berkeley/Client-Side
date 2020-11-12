#include <loadCell.h>

float data[6];

void setup() {
  // put your setup code here, to run once:
  LoadCell::init();
}

void loop() {
  // put your main code here, to run repeatedly:
  LoadCell::readLoad(data);
  for (int i = 0; i < 6; i++){
    Serial.print(data[i]);
    Serial.print(", ");
  }
  Serial.println("");
  delay(1000);
}
