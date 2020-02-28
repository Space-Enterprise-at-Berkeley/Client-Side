/*
  RS485.ino - Common code for handling RS485 communications.
  Created by Shao-Qian Mah, Jan 25, 2020.
*/

void internalTransmit(String *message) {
  // Set pin to high to have control over transmit.
  digitalWrite(RSCONTROL, HIGH);
  INTERNALSERIAL.print(*message);
  // Relinquish control.
  digitalWrite(RSCONTROL, LOW);
}
