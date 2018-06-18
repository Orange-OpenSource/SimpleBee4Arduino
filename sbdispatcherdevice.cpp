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
#include <sbdispatcherdevice.h>
#include <sbmessage.h>

  // ****** Public functions ******
/**
 * Device list that receive messages
 */
void SBDispatcherDevice::setDeviceList(SBDevice *devList[]) {
	this->deviceList = devList;
	for (SBDevice **device=deviceList;*device;device++) { // set sb messenger for all
		(*device)->sbmessenger = sbmessenger;
	}
};


/**
 * Dispatcher message incoming form SBMessager
 */
void SBDispatcherDevice::dispatch(char * msg, int len) {
	if (deviceList) {
		SBDevice ** device=deviceList;
		if ( SBTResponseType(SBMsgReqType::identification) == msg[0]) { // Identification attribution
			for (;*device;device++) {
				if (SBDevice::state::ukn == (*device)->currentState) { // Search First One
					SBMessageIdentificationResponse *identMsg = (SBMessageIdentificationResponse *) msg;
					(*device)->setAddr(identMsg->address);
					noIdentInProgress = true; // Authorize next ident
					break;
				}
			}
		} else {
			// search targeted device
			SBMessageIdentificationResponse *message = (SBMessageIdentificationResponse *) msg;
			for (;*device;device++)  {
				int i=SBAddress_size-1;
				for ( ; i>=0; i--) {
					if (message->address[i] != (*device)->sbaddress[i]) {
						break; // Next one
					}
				}
				if (i<0) { // Found
					break;
				}
			}
			if (*device) { // Found !
				(*device)->newMessage(msg);
			}
		}
	}
}

/**
 * Check if Heart Beat message must be sent and if there is awaiting ident/address
 */
void SBDispatcherDevice::heartbeat(void) {
	if (deviceList) {
		unsigned long now = millis();
		for (SBDevice **device=deviceList;*device;device++) { // check address on remaining devices
			if (noIdentInProgress && SBDevice::state::started == (*device)->currentState) { // Ask ident for device
				SBMessageIdentificationReq ident(*device);
				sbmessenger->send(&ident, sizeof(SBMessageIdentificationReq));
				(*device)->currentState = SBDevice::state::ukn;
				noIdentInProgress = false; // Only One in progress
			} else if ( // heartbeat if device in running mode
					(SBDevice::state::ukn != (*device)->currentState
							&& SBDevice::state::started != (*device)->currentState
							&& SBDevice::state::idle != (*device)->currentState) &&
					now >= (*device)->heartbeat_time) { // Heartbeat Time
				(*device)->heartbeat_time= now + ((*device)->heartbeat_retry ? (*device)->heartbeat_retry: (*device)->heartbeat_period); // Next heartbeat
				(*device)->sendHeartBeat();
			}
		}
	}
}


  
