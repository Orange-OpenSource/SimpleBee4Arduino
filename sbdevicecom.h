/*
 * Copyright (C) 2015 Orange
 *
 * This software is distributed under the terms and conditions of the 'Apache-2.0'
 * license which can be found in the file 'LICENSE.txt' in this package distribution
 * or at 'http://www.apache.org/licenses/LICENSE-2.0'.
 */

/* Device commons definition for SimpleBee Protocol
 *
 * Version:     0.1.0
 * Created:     2015-02-24 by Franck Roudet
 */
#ifndef __SBDEVICECOM_H_
#define __SBDEVICECOM_H_


const int SBAddress_size=4;

/**
 * Message Request type.
 * Sensor/actuator/object to Open The Box
 */
struct SBMsgReqType {
	enum SBMsgReqType_enum : char {
		identification='I',
		watchdog='W',
		data='D',
		request='R',
	};
};


/**
 * Device type.
 * Sensor/actuator/object to Open The Box
 */
struct SBDeviceType {
	enum SBDeviceType_enum :char {
		actuator='A',
		sensor='C'
	};
};


#endif // __SBDEVICECOM_H_
