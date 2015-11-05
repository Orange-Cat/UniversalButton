//
// Title: Arduino UniversalButton Library
// Author: Orange Cat
// Date: 3-11-2015
//
// Description:
//   See UniversalButton.h for a complete description.
//
// License:
//   This firmware is released under the Creative Commons Attribution-ShareAlike 4.0
//   International license.
//     http://creativecommons.org/licenses/by-sa/4.0/
//
#include "UniversalButton.h"

UniversalButton::UniversalButton()
: pin_(0),
  pin_type_(kPinNone),
  button_pressed_min_(LOW),
  button_pressed_max_(LOW),
  click_ms_(kDefaultClickTime),
  double_click_ms_(kDefaultDoubleClickTime),
  medium_press_ms_(kDefaultMediumPressTime),
  long_press_ms_(kDefaultLongPressTime),
  read_pin_ms_(kDefaultReadPinTime),
  click_func_(NULL),
  double_click_func_(NULL),
  medium_press_func_(NULL),
  long_press_func_(NULL),
  state_(kStateIdle),
  last_check_time_(0),
  start_time_(0),
  is_trigger_mode_(false),
  is_medium_press_release_mode_(false),
  is_pressed_(false)
{
    return;
}

void UniversalButton::attachDigitalPin(int digital_pin)
{
  attachDigitalPin(digital_pin, true);
}

void UniversalButton::attachDigitalPin(int digital_pin, bool active_low)
{
  pin_ = digital_pin;
  pin_type_ = kPinDigital;

  if (active_low) {
    // button is pulled to ground when pressed, we enable pull-up resistors to
    // keep it high when not pressed
    button_pressed_min_ = LOW;
    button_pressed_max_ = LOW;
    pinMode(pin_, INPUT_PULLUP);
  }
  else {
    // button is pulled high externally when pressed
    button_pressed_min_ = HIGH;
    button_pressed_max_ = HIGH;
    pinMode(pin_, INPUT);
  }
}

void UniversalButton::attachAnalogPin(int analog_pin, uint16_t  min_press_val, uint16_t  max_press_val)
{
  pin_ = analog_pin;
  pin_type_ = kPinAnalog;

  pinMode(pin_, INPUT);
  digitalWrite(pin_, LOW);
  button_pressed_min_ = min_press_val;
  button_pressed_max_ = max_press_val;
}

void UniversalButton::setClickTime(unsigned int ms)
{
  click_ms_ = ms;
}

void UniversalButton::setDoubleClickTime(unsigned int ms)
{
  double_click_ms_ = ms;
}

void UniversalButton::setMediumPressTime(unsigned int ms)
{
  medium_press_ms_ = ms;
}

void UniversalButton::setLongPressTime(unsigned int ms)
{
  long_press_ms_ = ms;
}

void UniversalButton::setTriggerMode(bool turn_on)
{
  is_trigger_mode_ = turn_on;
}

void UniversalButton::setMediumPressReleaseMode(bool turn_on)
{
  is_medium_press_release_mode_ = turn_on;
}

void UniversalButton::setReadPinTime(unsigned int ms)
{
  read_pin_ms_ = ms;
}

void UniversalButton::setClickCallback(UniversalButtonCallback function)
{
  click_func_ = function;
}

void UniversalButton::setDoubleClickCallback(UniversalButtonCallback function)
{
  double_click_func_ = function;
}

void UniversalButton::setMediumPressCallback(UniversalButtonCallback function)
{
  medium_press_func_ = function;
}

void UniversalButton::setLongPressCallback(UniversalButtonCallback function)
{
  long_press_func_ = function;
}

bool UniversalButton::isPressed()
{
  return is_pressed_;
}

void UniversalButton::update()
{
  unsigned long now = millis();

  if (now > last_check_time_ + read_pin_ms_) {
    last_check_time_ = now;

    uint16_t button_read;
    bool button_pressed;

    switch (pin_type_) {
      case kPinDigital:
        button_read = digitalRead(pin_);
        button_pressed = (button_read >= button_pressed_min_ && button_read <= button_pressed_max_);
        break;

      case kPinAnalog:
        button_read = analogRead(pin_);
        button_pressed = (button_read >= button_pressed_min_ && button_read <= button_pressed_max_);
        break;

      case kPinNone:
      default:
        button_pressed = false;
        break;
    }

    update(button_pressed);
  }
}

void UniversalButton::update(bool button_pressed)
{
  unsigned long now = millis();

  // the universal button state machine
  switch (state_) {
    case kStateIdle:
      // we only enter this state with the button not pressed (except possibly on the first
      // call if the button happens to be pressed)
      is_pressed_ = false;
      if (button_pressed) {
        state_ = kStatePress;
        start_time_ = now;  // remember starting time
      }
      break;

    case kStatePress:
      // we stay in this state until the button is released or it's held long enough to be a
      // medium or long button press
      if (!button_pressed) {
        if (now < start_time_ + click_ms_) {
          // button was released too quickly, so just debouncing, go back to idle
          state_ = kStateIdle;
        }
        else {
          // button was pressed long enough to be counted, and has now been released
          is_pressed_ = false;
          state_ = kStateClickRelease;
        }
      }
      else {
        // button is pressed
        if (now > start_time_ + medium_press_ms_) {
          // medium press function is called as soon as time is reached
          if (medium_press_func_ != NULL && !is_medium_press_release_mode_) {
            medium_press_func_();
          }
          state_ = kStateReachedMedium;
        }
        else if (now > start_time_ + click_ms_) {
          if (!button_pressed && click_func_ != NULL && is_trigger_mode_) {
            // first time we are here and it's tigger mode so invoke callback now
            click_func_();
          }
          is_pressed_ = true;
        }
        else {
          // still waiting in this state
        }
      }
      break;

    case kStateClickRelease:
      // button was pressed and now released, if we have a double click callback we wait for button to be
      // pressed a second time or to timeout to report single click (when not in trigger mode). If we do not
      // have a double click callback registered, then we are done as we can count this as a single click
      // immediately.
      if ((double_click_func_ == NULL) || (now > start_time_ + double_click_ms_)) {
        // no need to wait for double-click or we timed-out waiting for double click -- was a single click
        if (click_func_ != NULL && !is_trigger_mode_) {
          click_func_();
        }
        state_ = kStateIdle;
      }
      else if (button_pressed) {
        // this is a double click -- it's safe to report now before released
        is_pressed_ = true;
        double_click_func_();
        state_ = kStateReleaseFinal;
      }
      else {
        // still waiting in this state
      }
      break;

    case kStateReachedMedium:
      // we have reached the medium time event
      if (!button_pressed) {
        if (medium_press_func_ != NULL && is_medium_press_release_mode_) {
          medium_press_func_();
        }
        is_pressed_ = false;
        state_ = kStateIdle;
      }
      else if (now > start_time_ + long_press_ms_) {
        if (long_press_func_ != NULL) {
          long_press_func_();
        }
        state_ = kStateReleaseFinal;
      }
      else {
          // still waiting in this state
      }
      break;

    case kStateReleaseFinal:
      // we are done interpreting and sending events, just wait for the button
      // to be released and then go back to idle
      if (!button_pressed) {
        is_pressed_ = false;
        state_ = kStateIdle;
      }
      break;
  }
}

