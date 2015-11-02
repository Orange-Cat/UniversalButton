//
// Title: Arduino UniversalButton Library: I2cButton Example
// Author: Orange Cat
// Date: 3-11-2015
//
// Description:
//   A simple example showing how we can detect (debounced) buttons with I2C connected buttons.
//   This example uses the Adafruit RBG LCD Shield. Only the right and select buttons are decoded in
//   this example for simplicity.
//   Button presses are logged to the LCD display.
//   Detected presses are:
//    right single click,
//    right double click,
//    select single click, and
//    select long press (6 seconds or more).
//
// License:
//   This firmware is released under the Creative Commons Attribution-ShareAlike 4.0
//   International license.
//     http://creativecommons.org/licenses/by-sa/4.0/
//
#include <UniversalButton.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>

// our buttons
UniversalButton right;
UniversalButton select;

// the shield with I2C connected buttons
Adafruit_RGBLCDShield lcd;


void setup()
{
  // add our functions to right which will be called when a single click or double click is detected:
  right.setClickCallback(rightClick);
  right.setDoubleClickCallback(rightDoubleClick);

  // add our functions to select which will be called when a single click or long press is detected:
  select.setClickCallback(selectClick);
  select.setLongPressCallback(selectLongPress);

  // setup the lcd
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UniversalButton");
}

void loop()
{
  // we call update() each time through the loop with the state for each of our buttons.
  // readButtons() gets the button status via I2C from the sheild.
  int buttons = lcd.readButtons();
  right.update(buttons & BUTTON_RIGHT);
  select.update(buttons & BUTTON_SELECT);
}

void rightClick()
{
  // this function we passed to setClickCallback() gets called when the right button is
  // clicked for a short time
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("right click");
}

void rightDoubleClick()
{
  // this function we passed to setDoubleClickCallback() gets called when the right button is
  // double-clicked quickly
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("right double-click");
}

void selectClick()
{
  // this function we passed to setClickCallback() gets called when the select button is
  // clicked for short time
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("select click");
}

void selectLongPress()
{
  // this function we passed to setLongPressCallback() gets called when a select is held for
  // 6 seconds or more
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("select long press");
}

