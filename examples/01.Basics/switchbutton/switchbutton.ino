/*
 * Copyright (C) 2015 Orange
 *
 * This software is distributed under the terms and conditions of the 'Apache-2.0'
 * license which can be found in the file 'LICENSE.txt' in this package distribution
 * or at 'http://www.apache.org/licenses/LICENSE-2.0'.
 */

/* Switch button example
 *
 * Version:     0.1.0
 * Created:     2015-03-02 by Franck Roudet
 */
#include <SBMessenger.h>
#include <sbmessage.h>
#include <sbdevice.h>
#include <sbdispatcherdevice.h>

#include "SwitchPinout.h"
#include "PushPinout.h"


/**
  * Dispatcher for incoming Simple Bee message 
  */
SBDispatcherDevice disp;


/**
 * Simple bee message manager
 */
SBMessenger sbmessenger(Serial,SBEndOfMessage, &disp, SBCheckSum);



/**
 * Declare my devices
 */
MySwitch  switch1(11);          // switch1 on pin11 - default heart type
MySwitch  switch2(10, 120000);  // switch2 on pin10 - 2 min en milli secondes
MyPush    push1(9);             // push1 on pin9 - default heart type

/**
 * List of monitoring devices
 */
SBDevice * stbDeviceList[]={ &switch1, &switch2, &push1, NULL }; // WARNING: must ends with NULL


/**
 * Sensor list for checking Hardware;
 */ 
CheckChangeMixin * stbDeviceHardwareList[]={ &switch1, &switch2, &push1, NULL }; // WARNING: must ends with NULL

void setup() {
  // Start the serial port
  Serial.begin(57600);
  
  // Set list of device for dispatching
  disp.setDeviceList(stbDeviceList);

  //Serial.print("-starting\r");

  delay(500); // Waiting radio
  
  // Start device if necessary ia ask for address
  // start all (WARNING OTB don't do that !!!!!! so set addr
  //for (SBDevice **device=stbDeviceList;*device;device++) {
  //  (*device)->start();
  //}

  // set addresses
  switch1.setAddr("2001");
  switch2.setAddr("2002");
  push1.setAddr("1001");
}


void loop() {
  // Looks for incoming message
  sbmessenger.monitor();

  // Check for Hardware changes
  for (CheckChangeMixin **device=stbDeviceHardwareList;*device;device++) {
    (*device)->checkChange();
  }
}

