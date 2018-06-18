/*
 * Copyright (C) 2015 Orange
 *
 * This software is distributed under the terms and conditions of the 'Apache-2.0'
 * license which can be found in the file 'LICENSE.txt' in this package distribution
 * or at 'http://www.apache.org/licenses/LICENSE-2.0'.
 */

/* LED example
 *
 * Version:     0.1.0
 * Created:     2015-02-25 by Franck Roudet
 */
#include <SBMessenger.h>
#include <sbmessage.h>
#include <sbdevice.h>
#include <sbdispatcherdevice.h>

#include "LedPinout.h"


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
const char * const ledType="001";
MyLed led1(13);               // led1 on pin13 - default heart type
MyLed led2(12, 3000);         // led2 on pin12 - 3sec

/**
 * List of monitoring devices
 */
SBDevice * stbDeviceList[]={ &led1, &led2, NULL }; // WARNING: must ends with NULL



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
  led1.setAddr("0101");
  led2.setAddr("0102");
  
  led1.value=1;
  led1.batteryLevel=5;
  led2.value=2;
  led2.batteryLevel=9;

}


void loop() {
  // Looks for incoming message
  sbmessenger.monitor();

}

