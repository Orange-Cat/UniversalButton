//
// Title: Arduino UniversalButton Library
// Author: Orange Cat
// Version: 1.0.1
// Date: 2015-11-05
//
// Description:
//   This is an Arduino library for interacting with any button or buttons. The buttons may be
//   attached to digital pins, analog pins (like proportional hall effect sensors), or multiple buttons
//   attached to an analog pin (using different resistors to VCC), or even buttons attached to I2C
//   or SPI or other buses.
//
//   UniversalButton does debouncing and can call attached callback functions for:
//    single click events,
//    double click events,
//    medium press events, and
//    long  press events.
//
//   By default, a single click is not detected until the button is released. This prevents us from
//   reporting a single click just before we detect a double click or a medium or long press. However
//   there are instances where it is useful to detect a single click immediately (like a camera shutter
//   button). Invoking setTriggerMode(true) will cause the callback to be called as soon as the click
//   time is reached. And the click time can be changed using setClickTime() which defaults to
//   40 milliseconds to provide deboucing.
//
//   By default, medium press events are reported as soon as the button is held for the medium press
//   time (default 1.5 seconds). However, this results in medium press events always being detected
//   during a long press. However, by invoking setMediumPressReleaseMode(true), we can delay the reporting
//   of medium press events until the button is released. This prevents a medium press event from being
//   detected early in a long press.
//
//   If you do not have a medium press callback, but do have a long press callback, then any button presses
//   between the medium press time and just less than the long press time will be discarded. If you do not want
//   this behaviour, you can use medium press instead and set the medium press time as suitable. Note that this
//   behaviour can also be used to set a maximum limit for single button presses.
//
//   Long presses are detected as soon as the longPressTime is reached (default 6 seconds).
//
//   There are various methods to support altering the timing of button clicks and presses.
//
// Change Log:
//   1.0: Initial release
//   1.0.1: improve response time for click when we don't have a registered double-click callback
//
// License:
//   This firmware is released under the Creative Commons Attribution-ShareAlike 4.0
//   International license.
//     http://creativecommons.org/licenses/by-sa/4.0/
//
#ifndef UNIVERSALBUTTON_H_
#define UNIVERSALBUTTON_H_

#include "Arduino.h"

typedef void (*UniversalButtonCallback)(void);

class UniversalButton {
  public:
    UniversalButton();

  public:
    // attach digital or analog pin to the button -- this is not mandatory,
    // one can call update with a button state (from an I2C attached button for instance),
    // however if a pin is attached, update needs no argument
    void attachDigitalPin(int digital_pin);
    void attachDigitalPin(int digital_pin, bool active_low);
    void attachAnalogPin(int analog_pin, uint16_t min_press_val, uint16_t max_press_val);

    // set functions which will be called when button events are detected
    void setClickCallback(UniversalButtonCallback function);
    void setDoubleClickCallback(UniversalButtonCallback function);
    void setMediumPressCallback(UniversalButtonCallback function);
    void setLongPressCallback(UniversalButtonCallback function);

    // Call update() every time through loop for buttons on analog and
    // digital pins
    void update();

    // For other buttons (e.g. buttons on i2c) call update(is_pressed) instead
    // (at least about every 10-20 milliseconds), sending the current state
    void update(bool is_pressed);

    // isPressed returns true if the button is currently pressed and has been
    // held down at least for ClickTime milliseconds it can be used to see if
    // multiple buttons are pressed simultaneously
    bool isPressed();

  public:
    // methods for setting time of presses
    void setClickTime(unsigned int ms);
    void setDoubleClickTime(unsigned int ms);
    void setMediumPressTime(unsigned int ms);
    void setLongPressTime(unsigned int ms);

    // method for enabling trigger mode (which calls the click callback function immediately upon
    // detection, instead of waiting for the button to be released). It is off by default as it
    // causes the click callback function to be invoked at the beginning of double click or a
    // medium or long press
    void setTriggerMode(bool turn_on);

    // By default, medium press events are reported as soon as the button is held for the medium press
    // time. However, this results in medium press events always being detected during a long press.
    // By invoking setMediumPressReleaseMode(true), we can delay the reporting of medium press events
    // until the button is released. This prevents a medium press event from being detected early in a
    // long press.
    void setMediumPressReleaseMode(bool turn_on);

    // method for setting the minimum time between reading of the pin
    void setReadPinTime(unsigned int ms);

  public:
    // default milliseconds for button clicks and presses, change using setClickTime(), setDoubleClickTime(),
    // setMediumPressTime() and setLongPressTime()
    static const unsigned int kDefaultClickTime = 40;
    static const unsigned int kDefaultDoubleClickTime = 300;
    static const unsigned int kDefaultMediumPressTime = 1500;
    static const unsigned int kDefaultLongPressTime = 6000;

    // milliseconds between reading pin, change using setReadPinTime()
    static const unsigned int kDefaultReadPinTime = 10;

  private:
    enum UniversalButtonState {
      kStateIdle,
      kStatePress,
      kStateClickRelease,
      kStateReachedMedium,
      kStateReleaseFinal
    };

    enum UniversalButtonPin {
      kPinNone,
      kPinDigital,
      kPinAnalog
    };

  private:
    // defines the pin and the analog or digital values between which indicate
    // the button was pressed
    int pin_;
    UniversalButtonPin pin_type_;
    uint16_t button_pressed_min_;
    uint16_t button_pressed_max_;

    // minimum milliseconds before buttons are detected
    unsigned int click_ms_;
    unsigned int double_click_ms_;
    unsigned int medium_press_ms_;
    unsigned int long_press_ms_;

    // minimum milliseconds between reading digital/analog pins
    unsigned int read_pin_ms_;

    // our callbacks
    UniversalButtonCallback click_func_;
    UniversalButtonCallback double_click_func_;
    UniversalButtonCallback medium_press_func_;
    UniversalButtonCallback long_press_func_;

    // current state info
    UniversalButtonState state_;
    unsigned long last_check_time_;
    unsigned long start_time_;
    bool is_trigger_mode_;
    bool is_medium_press_release_mode_;
    bool is_pressed_;
};

#endif  // UNIVERSALBUTTON_H_
