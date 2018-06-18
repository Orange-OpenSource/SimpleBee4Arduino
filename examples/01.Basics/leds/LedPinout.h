/*
 * Copyright (C) 2015 Orange
 *
 * This software is distributed under the terms and conditions of the 'Apache-2.0'
 * license which can be found in the file 'LICENSE.txt' in this package distribution
 * or at 'http://www.apache.org/licenses/LICENSE-2.0'.
 */
 
/**
 * Override Led to process events
 */
class MyLed : public SBLed { // A lots of things is already do by SBLed class : ie message analysis
public:
  int pin;
  /**
   * Contructors
   */
  MyLed(const int pin, unsigned long hearbeat_period_ms=500) : SBLed(hearbeat_period_ms) , pin(pin) {
      pinMode(pin, OUTPUT);
  };
  /**
   * Init comm and hardware when address is requested
   */
  virtual void start(void) {
    SBLed::start();
    digitalWrite(pin,HIGH);
  };
   
  /**
   * Init comm and hardware when address is knwown by device
   */
  virtual void setAddr(const char * newAddress) {
    SBLed::setAddr(newAddress);
    digitalWrite(pin,HIGH);
  };
  

  /**
   * on change from OTB
   */
  virtual void newMessage(char *message) {
    SBLed::newMessage(message); // Message analysis that set le value member
    digitalWrite(pin,value);
  };
};
