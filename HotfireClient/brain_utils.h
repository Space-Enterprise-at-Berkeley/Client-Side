/*
 * Brain_utils.h- A c++ program that uses I2C to establish communication between 
 * the sensors and valves inside to the rocket with the ground station. Able to send
 * data to the ground station via RF. Can receive and process commands sent from 
 * ground station. This contains additional functions and structs used in Brain_I2C.ino.
 * Created by Vainavi Viswanath, Aug 21, 2020.
 */



String make_packet(struct sensorInfo sensor);
uint16_t Fletcher16(uint8_t *data, int count);
void chooseValveById(int id, struct valveInfo *valve);

/*
 * Data structure to allow the conversion of bytes to floats and vice versa.
 */
union floatArrToBytes {
  char buffer[24];
  float sensorReadings[6];
} farrbconvert;

/*
 * Data structure to store all information relevant to a specific sensor type.
 */
struct sensorInfo {
  String name;
  int board_address;
  int id;
  int clock_freq;
};

/*
 * Data structure to store all information relevant to a specific valve.
 */
struct valveInfo {
  String name;
  int id;
  int (*openValve)();
  int (*closeValve)();
  void (*ackFunc)(float *data);
};


/*
 * Constructs packet in the following format: 
 * {<sensor_ID>,<data1>,<data2>, ...,<dataN>|checksum}
 */
String make_packet(int id) {
  String packet_content = (String)id;
  packet_content += ",";
  for (int i=0; i<6; i++) {
    float reading = farrbconvert.sensorReadings[i];
    if (reading != -1) {
      packet_content += (String)reading;
      packet_content += ",";
    } else {
      break;
    }
  }
  packet_content.remove(packet_content.length()-1); 
  int count = packet_content.length();
  char const *data = packet_content.c_str();
  uint16_t checksum = Fletcher16((uint8_t *) data, count);
  packet_content += "|";
  String check_ = String(checksum, HEX);
  while(check_.length() < 4){
    check_ = "0" + check_;
  }
  packet_content += check_;
  String packet = "{" + packet_content + "}";
  
  return packet;
}

/*
 * Decodes a packet sent from ground station in the following format:
 * {<valve_ID>,<open(1) or close(0)|checksum}
 * Populated the fields of the valve and returns the action to be taken
 */
int decode_received_packet(String packet, valveInfo *valve) {
  Serial.println(packet);
  int data_start_index = packet.indexOf(',');
  if(data_start_index == -1) {
    return -1;
  }
  int valve_id = packet.substring(1,data_start_index).toInt();
  const int data_end_index = packet.indexOf('|');
  if(data_end_index == -1) {
    return -1;
  }
  int action = packet.substring(data_start_index + 1,data_end_index).toInt();
  
  String checksumstr = packet.substring(data_end_index + 1, packet.length()-1);
  Serial.println(checksumstr);
  char checksum_char[5];
  checksumstr.toCharArray(checksum_char, 5);
//  char *checksum_char = checksumstr.c_str();
  uint16_t checksum = strtol(checksum_char, NULL, 16);
  Serial.print("check: ");
  Serial.println(checksum);
  
  const int count = packet.substring(1, data_end_index).length(); // sanity check; is this right? off by 1 error?
  char data[count+1];// = packet.substring(1,data_end_index).c_str();
  packet.substring(1, data_end_index).toCharArray(data, count + 1);
  
  uint16_t _check = Fletcher16((uint8_t *) data, count);
  if (_check == checksum) {
    Serial.println("Checksum correct, taking action");
    chooseValveById(valve_id, valve);
    return action;
  } else {
    return -1;
  }
}


/*
 * Calculates checksum for key values being sent to ground station:
 * sensor_ID and it's corresponding data points
 */
uint16_t Fletcher16(uint8_t *data, int count) {
  
  uint16_t sum1 = 0;
  uint16_t sum2 = 0;

  for (int index=0; index<count; index++) {
    if (data[index] > 0) {
      sum1 = (sum1 + data[index]) % 255;
      sum2 = (sum2 + sum1) % 255;
    }
  }
  return (sum2 << 8) | sum1;
}
