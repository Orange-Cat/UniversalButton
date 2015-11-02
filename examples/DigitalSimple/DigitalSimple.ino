//
// Title: Arduino UniversalButton Library: DigitalSimple Example
// Author: Orange Cat
// Date: 3-11-2015
//
// Description:
//   A simple example showing how we can detect a (debounced) button click on a digital pin.
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
  Serial.println("UniversalButton: DigitalSimple Example");

  // attach our digital pin to the button (default is active low, which is what we want)
  button.attachDigitalPin(buttonPin);

  // add our function which will be called when a single click is detected:
  button.setClickCallback(buttonClick);
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

