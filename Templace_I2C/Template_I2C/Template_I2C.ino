#include <Wire.h>
#include <dht.h>

#define dht_apin A0 

//on Arduino, I2C SCL is pin A5 and SDA is A4

dht DHT;

const int tempPin = A0;

int tempVal;
float temp;
float hum;

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  DHT.read11(dht_apin);

  tempVal = analogRead(tempPin);
  hum = DHT.humidity;
  Serial.print(hum);
  Serial.print(",");
  temp = DHT.temperature;
  Serial.println(temp);
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  //Wire.write("hello "); // respond with message of 6 bytes
  //Wire.write(temp);
  byte f = digitalRead(tempPin);
  Wire.write(f);
  // as expected by master
}
