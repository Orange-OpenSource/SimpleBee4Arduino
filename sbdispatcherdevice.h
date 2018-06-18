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
#ifndef SBDISPATCHERDEVICE_H_
#define SBDISPATCHERDEVICE_H_
#include <sbdevice.h>
#include <SBMessenger.h>
#include <SBDispatcherBase.h>

/**
 * Dispatch message destinated to STB Device
 */
class SBDispatcherDevice : public SBDispatcherBase {
private:
	SBDevice ** deviceList=0L;       // Device List must end with NULL
	bool noIdentInProgress=true;     // only one ident in progress
public:
	// ****** Public functions ******
	/**
	 * Device list that receive messages
	 */
	void setDeviceList(SBDevice *devList[]);

	/**
	 * Dispatcher message incoming form SBMessager
	 */
	virtual void dispatch(char * msg, int len);

	/**
	 * Heart Beat message and waiting ident
	 * Check if there is unidentified device
	 */
	virtual void heartbeat(void);

};


#endif // SBDISPATCHERDEVICE_H_
