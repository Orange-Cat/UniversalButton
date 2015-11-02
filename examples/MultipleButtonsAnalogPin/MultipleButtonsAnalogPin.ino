//
// Title: Arduino UniversalButton Library: MultipleButtonsAnalogPin Example
// Author: Orange Cat
// Date: 3-11-2015
//
// Description:
//   An example showing how we can detect multiple buttons attached to a single analog pin through different
//   resistors to VCC.
//   Button presses are logged to the Serial Monitor at 9600 baud.
//
// License:
//   This firmware is released under the Creative Commons Attribution-ShareAlike 4.0
//   International license.
//     http://creativecommons.org/licenses/by-sa/4.0/
//
#include <UniversalButton.h>

// the analog pin all our buttons are connected to
static const int buttonPin = A0;

// the resistors result in ADC counts on the analog pin which are close to these values:
const unsigned int kRight = 0;
const unsigned int kUp = 145;
const unsigned int kDown = 329;
const unsigned int kLeft = 505;
const unsigned int kSelect = 741;
// however we allow a tollerance of this many ADC counts
const unsigned int kTolerance = 10;

// the five buttons attached to our single analog pin:
UniversalButton up;
UniversalButton down;
UniversalButton right;
UniversalButton left;
UniversalButton select;


void setup()
{
  Serial.begin(9600);
  Serial.println("UniversalButton: MultipleButtonAnalogPin Example");

  // set setup the right pin to be detected when ADC value is betweek kRight and kRight + kTolerance
  right.attachAnalogPin(buttonPin, kRight, kRight + kTolerance);
  // and add our callback so that rightClick() is called when we detect this button has been clicked:
  right.setClickCallback(rightClick);

  // configure and setup the other buttons:
  up.attachAnalogPin(buttonPin, kUp - kTolerance, kUp + kTolerance);
  up.setClickCallback(upClick);

  down.attachAnalogPin(buttonPin, kDown - kTolerance, kDown + kTolerance);
  down.setClickCallback(downClick);

  left.attachAnalogPin(buttonPin, kLeft - kTolerance, kLeft + kTolerance);
  left.setClickCallback(leftClick);

  // we also detect a long press on the select button
  select.attachAnalogPin(buttonPin, kSelect - kTolerance, kSelect + kTolerance);
  select.setClickCallback(selectClick);
  select.setLongPressCallback(selectLongPress);
}

void loop()
{
  // we update the status of all our buttons each time through the loop, the callback functions we setup will be
  // called if that button press is detected
  right.update();
  up.update();
  down.update();
  left.update();
  select.update();
}

void rightClick()
{
  Serial.println("right click");
}

void upClick()
{
  Serial.println("up click");
}

void downClick()
{
  Serial.println("down click");
}

void leftClick()
{
  Serial.println("left click");
}

void selectClick()
{
  Serial.println("select click");
}

void selectLongPress()
{
  Serial.println("select long press");
}
