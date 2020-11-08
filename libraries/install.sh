#!/usr/bin/env bash
rm -rf ~/Arduino/libraries/Barometer ~/Arduino/libraries/GPS ~/Arduino/libraries/IMU ~/Arduino/libraries/loadCell ~/Arduino/libraries/pressureTransducer ~/Arduino/libraries/tempController ~/Arduino/libraries/thermocouple
cp -rf * ~/Arduino/libraries/
