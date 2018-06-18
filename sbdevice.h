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
#ifndef __SBDEVICE_H_
#define __SBDEVICE_H_
#include "sbdevicecom.h"
#include "SBMessenger.h"

/**
 * Base class for Simple Bee Device
 */
class SBDevice {
public:
	char sbaddress[SBAddress_size];
	char deviceType;
	const char *moduleType;
	unsigned long heartbeat_time;   // next time to emit heartbeat (ms)
	unsigned long heartbeat_period; // heart_beat period in milliseconds
	unsigned long heartbeat_retry;  // heart_beat retry period in milliseconds
	unsigned char batteryLevel=9;   // Must be 0 empty to 9 full

	SBMessenger * sbmessenger;      // Message manager

	/**
	 * State machine type
	 */
	struct state {
		enum state_enum : char {
			idle=' ',       // Inactive device
			started='S',    // Need an address
			ukn='U',        // Address in progress
			identified='I', // address OK
			newvalue='V',
			valueack='v',
		};
	};


	char currentState = SBDevice::state::idle;

	/**
	 * Contructors
	 * Default module type is LED 0001
	 */
	SBDevice(const char * moduleType="001") { this->moduleType = moduleType; currentState = SBDevice::state::idle;}

	/**
	 * Address attribution
	 */
	void setAddr(const char * newAddress);

	/**
	 * Start device and so ask for an address
	 */
	virtual void start(void) {
		this->currentState = SBDevice::state::started;
	}

	/**
	 * Send Heart Beat message
	 */
	virtual void sendHeartBeat(void)=0;

	/**
	 * Incoming message for the device
	 */
	virtual void newMessage(char *message)=0;
};


/**
 * Base class for actuator
 * Les actionneurs envoient toutes les 500 ms une requête, ils attendent pendant 100 ms une réponse.
 */
class SBActuator : public SBDevice {
public:
	/**
	 * Contructors
	 */
	SBActuator(const char * moduleType=0L, unsigned long hearbeat_period_ms=500, unsigned long heartbeat_retry_ms=0 /* No retry */) : SBDevice(moduleType) {
		deviceType=SBDeviceType::actuator;
		heartbeat_period=hearbeat_period_ms;
		heartbeat_retry=heartbeat_retry_ms;
	}

	/**
	 * Send Heart Beat message
	 */
	//virtual void sendHeartBeat(void);

};


/***
 * Actuators
 */

/**
 * SimpleBee Binary State Actuator
 */
class SBBinaryStateActuator: public SBActuator {
public:
	char value; // 0 or 1

	/**
	 * Contructors
	 */
	SBBinaryStateActuator(const char * moduleType="001" /* Led by default */ , unsigned long hearbeat_period_ms=500) : SBActuator(moduleType, hearbeat_period_ms) {};

	/**
	 * Send Heart Beat message
	 */
	virtual void sendHeartBeat(void);

	/**
	 * Incoming message for the device
	 */
	virtual void newMessage(char *message);
};

class SBLed: public SBBinaryStateActuator {
public:
	/**
	 * Contructors
	 */
	SBLed(unsigned long hearbeat_period_ms=500) : SBBinaryStateActuator("001", hearbeat_period_ms) {};
};





/**
 * Base class for Sensor
 * Les capteurs envoient leur valeurs si changement d’état. Ils essayent pendant 2 seconde jusqu’à obtenir un acquittement, sinon attendent le prochain changement d’état.
 * Les capteurs se signalent par un message toutes les minutes, si pas de réponse, ils recommencent toutes les 2 secondes jusqu’à réponse correcte.
 */
class SBSensor : public SBDevice {
public:
	/**
	 * Contructors
	 */
	SBSensor(const char * moduleType=0L, unsigned long hearbeat_period_ms=60000 /* 1 min */, unsigned long heartbeat_retry_ms=2000) : SBDevice(moduleType) {
		deviceType=SBDeviceType::sensor;
		heartbeat_period=hearbeat_period_ms;
		heartbeat_retry=heartbeat_retry_ms;
	}

	/**
	 * Send Message Data
	 * Les capteurs envoient leur valeurs si changement d’état. Ils essayent pendant 2 seconde jusqu’à obtenir un acquittement, sinon attendent le prochain changement d’état
	 */
	virtual void sendMessageData(void)=0;

};

/**
 * SimpleBee Binary State Sensor
 */
class SBBinaryStateSensor: public SBSensor {
public:
	char value; // 0 or 1

	/**
	 * Contructors
	 */
	SBBinaryStateSensor(const char * moduleType="002", unsigned long hearbeat_period_ms=60000 /* 1 min */) : SBSensor(moduleType, hearbeat_period_ms) {};

	/**
	 * Send Heart Beat message
	 */
	virtual void sendHeartBeat(void);

	/**
	 * Incoming message for the device
	 */
	virtual void newMessage(char *message);

	/**
	 * Send Message Data
	 */
	virtual void sendMessageData(void);

};


/**
 * SimpleBee Push button
 * 2 state device = 0 and 1
 */
class SBPush: public SBBinaryStateSensor {
public:
	/**
	 * Contructors
	 */
	SBPush(unsigned long hearbeat_period_ms=60000 /* 1 min */) : SBBinaryStateSensor("001", hearbeat_period_ms) {};

};

/**
 * SimpleBee Switch
 * 2 state device = 0 and 1
 */
class SBSwitch: public SBBinaryStateSensor {
public:
	/**
	 * Contructors
	 */
	SBSwitch(unsigned long hearbeat_period_ms=60000 /* 1 min */) : SBBinaryStateSensor("002", hearbeat_period_ms) {};

};

#endif // __SBDEVICE_H_
