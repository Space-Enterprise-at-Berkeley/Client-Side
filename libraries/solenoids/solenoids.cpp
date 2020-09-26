/*
  solenoids.cpp - A c++ library to interface with all of the solenoids on our rocket.
  Created by Vamshi Balanaga, Sept 15, 2020.
*/

#include "solenoids.h"
#include "Arduino.h"

// ==================== NEED TO TEST =======================================

Solenoids::Solenoids() {
}

void Solenoids::init() {
  lox2_state = 0;
  lox5_state = 0;
  lox_gems_state = 0;

  prop2_state = 0;
  prop5_state = 0;
  prop_gems_state = 0;

  high_sol_state = 0;

  pinMode(LOX_2_PIN, OUTPUT);
  pinMode(LOX_5_PIN, OUTPUT);
  pinMode(LOX_GEMS_PIN, OUTPUT);

  pinMode(PROP_2_PIN, OUTPUT);
  pinMode(PROP_5_PIN, OUTPUT);
  pinMode(PROP_GEMS_PIN, OUTPUT);

  pinMode(HIGH_SOL_PIN, OUTPUT);

  digitalWrite(LOX_2_PIN, lox2_state);
  digitalWrite(LOX_5_PIN, lox5_state);
  digitalWrite(LOX_GEMS_PIN, lox_gems_state);

  digitalWrite(PROP_2_PIN, prop2_state);
  digitalWrite(PROP_5_PIN, prop5_state);
  digitalWrite(PROP_GEMS_PIN, prop_gems_state);

  digitalWrite(HIGH_SOL_PIN, high_sol_state);
}

int Solenoids::toggleHighPressureSolenoid() {
  if(high_sol_state == 0){
    high_sol_state = 1;
  } else {
    high_sol_state = 0;
  }
  digitalWrite(HIGH_SOL_PIN, high_sol_state);
  return high_sol_state;
}

int Solenoids::toggleLOX2Way() {
  if (lox2_state == 0){
    lox2_state = 1;
  } else {
    lox2_state = 0;
  }
  digitalWrite(LOX_2_PIN, lox2_state);
  return lox2_state;
}

int Solenoids::toggleLOX5Way() {
  if (lox5_state == 0){
    lox5_state = 1;
  } else {
    lox5_state = 0;
  }
  digitalWrite(LOX_5_PIN, lox5_state);
  return lox5_state;
}

int Solenoids::toggleLOXGems() {
  if (lox_gems_state == 0){
    lox_gems_state = 1;
  } else {
    lox_gems_state = 0;
  }
  digitalWrite(LOX_GEMS_PIN, lox_gems_state);
  return lox_gems_state;
}

int Solenoids::toggleProp2Way() {
  if (prop2_state == 0){
    prop2_state = 1;
  } else {
    prop2_state = 0;
  }
  digitalWrite(PROP_2_PIN, prop2_state);
  return prop2_state;
}

int Solenoids::toggleProp5Way() {
  if (prop5_state == 0){
    prop5_state = 1;
  } else {
    prop5_state = 0;
  }
  digitalWrite(PROP_5_PIN, prop5_state);
  return prop5_state;
}

int Solenoids::togglePropGems() {
  if (prop_gems_state == 0){
    prop_gems_state = 1;
  } else {
    prop_gems_state = 0;
  }
  digitalWrite(PROP_GEMS_PIN, prop_gems_state);
  return prop_gems_state;
}

int Solenoids::activateHighPressureSolenoid() {
  if(high_sol_state == 0){
    toggleHighPressureSolenoid();
  } else {
    // already active, do nothing.
  }
  return high_sol_state;
}

int Solenoids::armLOX() {
  if (lox2_state == 0) {
    toggleLOX2Way();
  } else {
    // already active, do nothing.
  }
  return lox2_state;
}

int Solenoids::disarmLOX() {
  if (lox2_state == 1) {
    toggleLOX2Way();
  } else {
    // already closed, do nothing.
  }
  return lox2_state;
}

int Solenoids::armPropane() {
  if (prop2_state == 0) {
    toggleProp2Way();
  } else {
    // already active, do nothing.
  }
  return prop2_state;
}

int Solenoids::disarmPropane() {
  if (prop2_state == 1) {
    toggleProp2Way();
  } else {
    // already closed, do nothing.
  }
  return prop2_state;
}

int Solenoids::armAll() {
  return armLOX() & armPropane();
}

int Solenoids::disarmAll(){
  return (!disarmLOX() & !disarmPropane());
}

int Solenoids::openLOX() {
  if(!loxArmed()){
    return -1;
  }
  if (lox5_state == 0) {
    toggleLOX5Way();
  } else {
    // already active, do nothing.
  }
  return lox5_state;
}

int Solenoids::closeLOX() {
  if(!loxArmed()){ // it won't work if there's no pressure. ie even to close, needs to be armed.
    return -1;
  }
  if (lox5_state == 1) {
    toggleLOX5Way();
  } else {
    // already closed, do nothing.
  }
  return lox5_state;
}

int Solenoids::openPropane() {
  if(!propArmed()) {
    return -1;
  }
  if (prop5_state == 0) {
    toggleProp5Way();
  } else {
    // already active, do nothing.
  }
  return prop5_state;
}

int Solenoids::LAUNCH() {
  if (!loxArmed() || !propArmed()){
    return -1;
  }
  openLOX();
  openPropane();
  return 1;
}

bool Solenoids::loxArmed() {
  return lox2_state == 1;
}

bool Solenoids::propArmed() {
  return prop2_state == 1;
}
