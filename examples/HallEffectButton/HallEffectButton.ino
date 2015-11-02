//
// Title: Arduino UniversalButton Library: HallEffectButton Example
// Author: Orange Cat
// Date: 3-11-2015
//
// Description:
//   An example showing how we can use a magnet and a proportional hall effect sensor connected to an
//   analog pin to detect button presses where the magnet moves closer to the hall effect sensor when
//   pressed.
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

// when the button is pressed the ADC count from the hall effect sensor decreases, the
// minimum and maximum ADC counts that represent button press:
const unsigned int kMin = 85;
const unsigned int kMax = 165;

// our button
UniversalButton hallEffect;


void setup()
{
  Serial.begin(9600);
  Serial.println("UniversalButton: HallEffectButton Example");

  // setup our hall effect button
  hallEffect.attachAnalogPin(buttonPin, kMin, kMax);

  // and a function which will be called when we detect the button was clicked:
  hallEffect.setClickCallback(click);

  // and another function which will be called when we detect the button was double-clicked:
  hallEffect.setDoubleClickCallback(doubleClick);

  // and a final function which will be called when we detect the button was held for a long time:
  hallEffect.setLongPressCallback(longPress);
}

void loop()
{
  // we call update() each time through the loop to process the button clicks and call the callback
  // functions when a particular button press has been detected
  hallEffect.update();
}

void click()
{
  Serial.println("click");
}

void doubleClick()
{
  Serial.println("double-click");
}

void longPress()
{
  Serial.println("long press");
}

