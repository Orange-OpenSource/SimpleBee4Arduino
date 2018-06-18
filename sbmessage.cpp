/*
 * Copyright (C) 2015 Orange
 *
 * This software is distributed under the terms and conditions of the 'Apache-2.0'
 * license which can be found in the file 'LICENSE.txt' in this package distribution
 * or at 'http://www.apache.org/licenses/LICENSE-2.0'.
 */

/* Message Type for SimpleBee Protocol
 *
 * Version:     0.1.0
 * Created:     2015-02-18 by Franck Roudet
 */

#include "sbmessage.h"
#include "sbdevice.h"
/*
 * Calc checksum and store in dest.
 * dest size is 2
 */
void SBCheckSum(const char * start, int length, char * dest) {
	unsigned char csum=0;
	const char * stop = start+ length;
	while (start < stop) {
		csum +=(unsigned char) *start++;
	}
	dest[0] = ((csum & 0xF0) >> 4) + '0';
	dest[1] = (csum & 0x0F)        + '0';
}

SBMessageIdentificationReq::SBMessageIdentificationReq(const SBDevice * const device): SBMessageIdentificationReq(device->moduleType) {
	this->deviceType=device->deviceType;
}



/**
 * Actuator
 */

SBBinaryStateMessageRequestReq::SBBinaryStateMessageRequestReq(const SBBinaryStateActuator * const device) :
		SBBinaryStateMessageRequestReq(device->sbaddress, device->value, device->batteryLevel) {
	memcpy(this->moduleType, device->moduleType, MODULE_TYPE_SIZEOF);
};


/**
 * Sensor
 */
SBBinaryStateBaseMessageReq::SBBinaryStateBaseMessageReq(const char messageType, const SBBinaryStateSensor * const device): SBMessage(messageType) {
	this->batterylevel= '0' + (device->batteryLevel % 10);
	this->value= '0' + device->value;
	memcpy(this->sbaddress,  device->sbaddress,  ADR_TYPE_SIZEOF);
	memcpy(this->moduleType, device->moduleType, MODULE_TYPE_SIZEOF);
}

SBBinaryStateMessageWatchdogReq::SBBinaryStateMessageWatchdogReq(const SBBinaryStateSensor * const device):SBBinaryStateBaseMessageReq(SBMsgReqType::watchdog,device) {
}

SBBinaryStateMessageDataReq::SBBinaryStateMessageDataReq(const SBBinaryStateSensor * const device):SBBinaryStateBaseMessageReq(SBMsgReqType::data,device) {
}

/**
 * Switch message
 */

