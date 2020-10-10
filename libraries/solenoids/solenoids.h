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
    void init();

    int activateHighPressureSolenoid();
    int deactivateHighPressureSolenoid();

    int ventPropaneGems();
    int ventLOXGems();
    int closePropaneGems();
    int closeLOXGems();

    int armLOX();
    int armPropane();
    int armAll();

    int disarmLOX();
    int disarmPropane();
    int disarmAll();

    int openLOX();
    int openPropane();

    int closeLOX();
    int closePropane();

    int LAUNCH();


        int toggleHighPressureSolenoid();

        int toggleLOX2Way();
        int toggleLOX5Way();
        int toggleLOXGems();

        int toggleProp2Way();
        int toggleProp5Way();
        int togglePropGems();

        bool loxArmed();
        bool propArmed();

  private:
    int high_sol_state = 0;

    int lox2_state = 0;
    int lox5_state = 0;
    int lox_gems_state = 0;

    int prop2_state = 0;
    int prop5_state = 0;
    int prop_gems_state = 0;

};
