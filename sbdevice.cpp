/*
 * Copyright (C) 2015 Orange
 *
 * This software is distributed under the terms and conditions of the 'Apache-2.0'
 * license which can be found in the file 'LICENSE.txt' in this package distribution
 * or at 'http://www.apache.org/licenses/LICENSE-2.0'.
 */

/* Device for SimpleBee Protocol
 *
 * Version:     0.1.0
 * Created:     2015-02-24 by Franck Roudet
 */


#include <sbdevice.h>
#include <sbmessage.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

/**
 * Address attribution
 */
void SBDevice::setAddr(const char * newAddress ) {
	for (int i=0; i< SBAddress_size; ++i) {
		sbaddress[i] = newAddress[i];
	}
	this->currentState = SBDevice::state::identified;
	heartbeat_time = millis() + heartbeat_period;
}


/******************************************
 *           Actuators
 ******************************************/


/**
 * SBBinaryStateActuator, and SBLed
 */

/**
 * Send Heart Beat message
 */
void SBBinaryStateActuator::sendHeartBeat(void) {
	SBBinaryStateMessageRequestReq msg(this);
	sbmessenger->send(&msg, sizeof(SBBinaryStateMessageRequestReq));
	this->currentState = SBDevice::state::newvalue;
}


/**
 * Incoming message for the device
 */
void SBBinaryStateActuator::newMessage(char *message) {
	SBMessageRequestResponse *rep=(SBMessageRequestResponse*)message;
	value = (rep->value - '0') % 2; // Force status to be 0 or 1
	this->currentState = SBDevice::state::identified;
	heartbeat_time = millis() + heartbeat_period;
}


/******************************************
 *           Sensors
 ******************************************/



/**
 * Binary state sensor switch
 */

/**
 * Send Heart Beat message
 */
void SBBinaryStateSensor::sendHeartBeat(void) {
	SBBinaryStateMessageWatchdogReq msg(this);
	sbmessenger->send(&msg, sizeof(SBBinaryStateMessageWatchdogReq));
	this->currentState = SBDevice::state::newvalue;
}


/**
 * Incoming message for the device
 */
void SBBinaryStateSensor::newMessage(char *message) {
	SBBinaryStateMessageWatchdogResponse *rep=(SBBinaryStateMessageWatchdogResponse*)message;
	value = (rep->value - '0') % 2;
	this->currentState = SBDevice::state::identified;
	heartbeat_time = millis() + heartbeat_period;
}


/**
 * Send Message Data
 */
void SBBinaryStateSensor::sendMessageData(void) {
	SBBinaryStateMessageDataReq msg(this);
	sbmessenger->send(&msg, sizeof(SBBinaryStateMessageDataReq));
	this->currentState = SBDevice::state::newvalue;
	heartbeat_time = millis() + heartbeat_period;
}


