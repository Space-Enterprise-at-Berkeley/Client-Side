# Client-Side
All the client side code for sensors, valves, IMUs and the rest of the fun stuff.

## Setup:

Download the [Arduino IDE](https://www.arduino.cc/en/Main/Software).

We use the Teensy 4.0 board in addition to vanilla arduino boards. Setup the environment according to the [instructions here](https://www.pjrc.com/teensy/td_download.html).

Copy all of the contents of the `libraries` directory into your Arduino IDE libraries directory.
You can find the path to your Arduino IDE library path by clicking on `File -> Preferences` in the Arduino IDE.

## Contribution

### Client Boards

*Duplicate the Template folder for a new client board.*

### Sensors

The template code houses the client side communications protocol for our systems.

Each sensor has its own `.h` file which can be included in any version of a client
code. The only requirement is that you provide a function of the form `read_data()`
that returns a struct with the current time and the sensor reading.

When you submit a pull request, include the average time your function takes in microseconds.

NOTE: This architecture is borrowed from McGill Rocket Team. See [here](https://github.com/liurichard95/mrt-avionics)
for their documentation.

### Submitting

Do all of your development on a feature branch named along the lines of `<your-name>_<sensor-name>`.
Try to commit frequently with descriptive messages. When you're done, push your code to your branch,
submit a pull request into `master` and request a review from at least one other person
on the team.
