
#define LOX_2 2
#define LOX_5 3
#define LOX_GEMS 4

#define PROP_2 8
#define PROP_5 9
#define PROP_GEMS 10


int lox2_state = LOW;
int lox5_state = LOW;
int lox_gems_state = LOW;

int prop2_state = LOW;
int prop5_state = LOW;
int prop_gems_state = LOW;

void setup() {

  Serial.begin(9600);

  pinMode(LOX_2, OUTPUT);
  pinMode(LOX_5, OUTPUT);
  pinMode(LOX_GEMS, OUTPUT);

  pinMode(PROP_2, OUTPUT);
  pinMode(PROP_5, OUTPUT);
  pinMode(PROP_GEMS, OUTPUT);

  digitalWrite(LOX_2, lox2_state);
  digitalWrite(LOX_5, lox5_state);
  digitalWrite(LOX_GEMS, lox_gems_state);

  digitalWrite(PROP_2, prop2_state);
  digitalWrite(PROP_5, prop5_state);
  digitalWrite(PROP_GEMS, prop_gems_state);

}

void loop() {
  if (Serial.available() > 0) {
      int readByte = Serial.read();
      if(readByte == 'a'){
        lox2_state = toggle(lox2_state);
        digitalWrite(LOX_2, lox2_state);
        Serial.print("Toggled LOX 2: ");
        Serial.println(lox2_state);
      } else if(readByte == 'b'){
        lox5_state = toggle(lox5_state);
        digitalWrite(LOX_5, lox5_state);
        Serial.print("Toggled LOX 5: ");
        Serial.println(lox5_state);
      } else if (readByte == 'c'){
        lox_gems_state = toggle(lox_gems_state);
        digitalWrite(LOX_GEMS, lox_gems_state);
        Serial.print("Toggled LOX Gems: ");
        Serial.println(lox_gems_state);
      } else if(readByte == 'x'){
        prop2_state = toggle(prop2_state);
        digitalWrite(PROP_2, prop2_state);
        Serial.print("Toggled PROP 2: ");
        Serial.println(prop2_state);
      } else if (readByte == 'y') {
        prop5_state = toggle(prop5_state);
        digitalWrite(PROP_5, prop5_state);
        Serial.print("Toggled PROP 5: ");
        Serial.println(prop5_state);
      } else if (readByte == 'z'){
        prop_gems_state = toggle(prop_gems_state);
        digitalWrite(PROP_GEMS, prop_gems_state);
        Serial.print("Toggled PROP Gems: ");
        Serial.println(prop_gems_state);
      }
    }
    delay(100);
}

int toggle(int input_state){
  if(input_state == LOW){
     input_state = HIGH;
  } else if (input_state == HIGH){
     input_state = LOW;
  }
  return input_state;
}
