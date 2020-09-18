/*
  solenoids.h - A c++ library to interface with all of the solenoids on our rocket.
  Created by Vamshi Balanaga, Sept 15, 2020.
*/


#define LOX_2_PIN 2
#define LOX_5_PIN 3
#define LOX_GEMS_PIN 4

#define PROP_2_PIN 8
#define PROP_5_PIN 9
#define PROP_GEMS_PIN 10

#define HIGH_SOL_PIN 13

using namespace std;

class Solenoids {
  public:
    Solenoids();
    int activateHighPressureSolenoid();
    int armLOX();
    int armPropane();
    int armAll();
    int openLOX();
    int openPropane();
    int LAUNCH();

  private:

    int lox2_state = 0;
    int lox5_state = 0;
    int lox_gems_state = 0;

    int prop2_state = 0;
    int prop5_state = 0;
    int prop_gems_state = 0;

    int high_sol_state = 0;

    void init();

    int toggleHighPressureSolenoid();
    int toggleLOX2Way();
    int toggleLOX5Way();
    int toggleProp2Way();
    int toggleProp5Way();

    bool loxArmed();
    bool propArmed();
}
