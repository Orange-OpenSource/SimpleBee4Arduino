/*
 * Copyright (C) 2015 Orange
 *
 * This software is distributed under the terms and conditions of the 'Apache-2.0'
 * license which can be found in the file 'LICENSE.txt' in this package distribution
 * or at 'http://www.apache.org/licenses/LICENSE-2.0'.
 */
 
 #include "checkChangeMixin.h"
 /**
 * Override Switch to process events
 */
class MySwitch : public SBSwitch, public CheckChangeMixin {
public:
  int pin;
  /**
   * Contructors
   */
  MySwitch(const int pin, unsigned long hearbeat_period_ms=60000) : SBSwitch(hearbeat_period_ms) , pin(pin) {
    pinMode(pin, INPUT);
    value=digitalRead(pin);  
  };
  

  /**
   * Heart beat
   */  
  virtual void sendHeartBeat(void) {
    value=digitalRead(pin);
    SBSwitch::sendHeartBeat();
  };


  /**
   * check hardware changes
   */  
  virtual void checkChange(void) {
    char actualValue = digitalRead(pin);
    if (actualValue!=value) {
       value=actualValue;
       sendMessageData();
    }
  }
};
