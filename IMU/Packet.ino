/*
  Packet.ino - Common packet handling code.
  Created by Shao-Qian Mah, Jan 25, 2020.
*/

// Read a packet coming in from internalSerial. 
// Check that incoming packet matches EXPECTEDID, and it doesn't time out.
// Return "" (empty string) if exceeds MAXCHARS, including '(' and ')' characters or TIMEOUT (in millis).
String readPacket(int expectedId, long startTime, int charMax, int timeOut) {
  String packet = "";
  int charsRcvd = 0;
  int receivedID = 0;
  bool started = false;
  digitalWrite(RSCONTROL, LOW);
  while (millis() - startTime <= timeOut) {
    if (INTERNALSERIAL.available() > 0)
    {
      char inChar = INTERNALSERIAL.read();
      if (charsRcvd >= charMax) {
        return "";
      }
      if (inChar == STARTCHAR)
      {
        started = true;
      }
      if (started)
      {
        charsRcvd += 1;
        // Always append inChar.
        packet += inChar;
      }
      // Check if expectedID.
      if (charsRcvd == 3) {
        int id = packet.substring(1).toInt();
        if (id != expectedId) {
          return "";
        }
      }
      if (inChar == ENDCHAR)
      {
        return packet;
      }
    }
  }
  return "";
}
