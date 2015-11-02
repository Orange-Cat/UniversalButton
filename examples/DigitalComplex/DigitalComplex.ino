//
// Title: Arduino UniversalButton Library: DigitalComplex Example
// Author: Orange Cat
// Date: 3-11-2015
//
// Description:
//   An example showing detection of all button clicks and presses and changing the timing of button presses.
//   Button presses are logged to the Serial Monitor at 9600 baud.
//
// License:
//   This firmware is released under the Creative Commons Attribution-ShareAlike 4.0
//   International license.
//     http://creativecommons.org/licenses/by-sa/4.0/
//
#include <UniversalButton.h>

// Button is attached directly between buttonPin and ground, by default we enable pullups on a
// digital pin, so no need for any other components
const int buttonPin = 4;

// our button
UniversalButton button;


void setup()
{
  // setup the serial port to log button presses
  Serial.begin(9600);
  Serial.println("UniversalButton: DigitalComplex Example");

  // attach our digital pin to the button (default is active low, which is what we want)
  button.attachDigitalPin(buttonPin);

  // add our functions which will be called when specific button presses are detected
  // we could choose to have functions for any or all of these:
  button.setClickCallback(buttonClick);
  button.setDoubleClickCallback(buttonDoubleClick);
  button.setMediumPressCallback(buttonMediumPress);
  button.setLongPressCallback(buttonLongPress);

  // we enable medium press release mode so that medium press is only detected when the button is
  // released thus preventing buttonMediumPress() from also being called during a long press.
  button.setMediumPressReleaseMode(true);

  // we set the minimum time to detect a long press to 4 seconds, the default is 6 seconds
  // (the maximum allowed is 16 seconds)
  button.setLongPressTime(4000);
}

void loop()
{
  // we call this each time through the loop to process the button clicks and call the callback
  // functions when a particular button press has been detected
  button.update();
}

void buttonClick()
{
  // this function we passed to setClickCallback() gets called when a button is clicked for
  // a short time
  Serial.println("button click");
}

void buttonDoubleClick()
{
  // this function we passed to setDoubleClickCallback() gets called when a button is clicked twice
  // rapidly
  Serial.println("button double click");
}

void buttonMediumPress()
{
  // this function we passed to setMediumPressCallback() gets called when a button is held for
  // a while (default is 1500 milliseconds, 1.5 seconds)
  Serial.println("button medium press");
}

void buttonLongPress()
{
  // this function we passed to setLongPressCallback() gets called when a button is held for
  // a long time (default is 6000 milliseconds, 6 seconds)
  Serial.println("button long press");
}
