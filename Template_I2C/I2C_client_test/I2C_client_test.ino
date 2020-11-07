#include <Wire.h>

void setup()
{
  Wire.begin(9);                // join i2c bus with address #9
  //Wire.setSDA(18);
  //Wire.setSCL(19);
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop()
{
//  while (Wire.available()) // loop through all but the last
//  {
//    char c = Wire.read(); // receive byte as a character
//    Serial.print(c);         // print the character
//  }
  Serial.println("checking wire available");
  while (!Wire.available());
  Serial.println("past wire available");
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
  delay(1000);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}

//#include <Arduino.h>
//#include <i2c_driver.h>
//#include <i2c_driver_wire.h>
//
//void setup()
//{
//  Wire1.begin(4);                // join i2c bus with address #4
//  Wire1.onReceive(receiveEvent); // register event
//  Serial.begin(9600);           // start serial for output
//}
//
//void loop()
//{
//  delay(100);
//}
//
//// function that executes whenever data is received from master
//// this function is registered as an event, see setup()
//void receiveEvent(int howMany)
//{
//  while(Wire1.available()) {  // loop through all but the last
//    char c = Wire1.read();        // receive byte as a character
//    Serial.print(c);             // print the character
//  }
//  Serial.println();
//  int x = Wire1.read();           // receive byte as an integer
//  Serial.println(x);             // print the integer
//}
