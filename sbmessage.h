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
#ifndef __SBMESSAGE_H_
#define __SBMESSAGE_H_
#include <sbdevicecom.h>
#include <string.h>



// Size of module address
#define ADR_TYPE_SIZEOF 4

#define SBTResponseType(SBMsgReq) (SBMsgReq | 0x20)


/**
 * Forward declaration
 */
class SBDevice;
class SBSensor;
class SBSwitch;
class SBBinaryStateSensor;
class SBBinaryStateActuator;

/*
 * Calc checksum and store in dest.
 * dest size is 2
 */
extern void SBCheckSum(const char * start, int length, char * dest);

const char SBEndOfMessage='\r';
const char MODULE_TYPE_SIZEOF=3;


/**
 * Base class for requests
 */
class SBMessage {
public:
	const char type;

	SBMessage(const char type):type(type) {}
};

/**
 * Request Message for identification.
 * Les modules neuf ont une adresse ‘0000’ et restent muets  à la mise sous tension.
 * Lorsqu’on appuie sur le petit bouton en bas à gauche, le module envoie un message de demande d’adresse une seule fois à chaque appuie. Si pas de réponse, il se rendort.
 * Si la réponse est correcte, il fonctionne normalement et commence à envoyer des données. L’adresse est conservée dans le module et celui-ci fonctionnera normalement à une prochaine mise sous tension.
 *
 */
class SBMessageIdentificationReq : public SBMessage {
public:

	char deviceType =SBDeviceType::sensor;
	char moduleType[MODULE_TYPE_SIZEOF];


	/**
	 * Contructors
	 */
	SBMessageIdentificationReq(const char *moduleType):SBMessage(SBMsgReqType::identification) {
		memcpy(this->moduleType, moduleType, MODULE_TYPE_SIZEOF);
	};
	SBMessageIdentificationReq(const SBDevice * const device);

};


class SBMessageIdentificationResponse : public SBMessage {
public:
	char address[ADR_TYPE_SIZEOF];

	/**
	 * Constructors
	 */
	SBMessageIdentificationResponse(const char *moduleType):SBMessage(SBTResponseType(SBMsgReqType::identification)) {};

};



/**
 * Request Message for Binary State Actuator (LED...).
 * Message request :
 * Les actionneurs envoient toutes les 500 ms une requête, ils attendent pendant 100 ms une réponse.
 *
 */
class SBBinaryStateMessageRequestReq : public SBMessage {
public:

	char sbaddress[ADR_TYPE_SIZEOF];
	char value;
	const char batDelimit='B';
	char batterylevel; // Must be '0' empty to '9' full

	char deviceType =SBDeviceType::actuator;
	char moduleType[MODULE_TYPE_SIZEOF];

	/**
	 * Contructors
	 */
	SBBinaryStateMessageRequestReq(const char *sbaddress):SBMessage(SBMsgReqType::request) {
		memcpy(this->sbaddress, sbaddress, ADR_TYPE_SIZEOF);
	};
	SBBinaryStateMessageRequestReq(const char *sbaddress, char value): SBBinaryStateMessageRequestReq(sbaddress) {
		this->value= '0' + (value % 2);
	};
	SBBinaryStateMessageRequestReq(const char *sbaddress, char value, char batterylevel): SBBinaryStateMessageRequestReq(sbaddress,value) {
		this->batterylevel= '0' + (batterylevel % 10);
	};

	SBBinaryStateMessageRequestReq(const SBBinaryStateActuator * const device);

};

/**
 * Request Response Message for Binary State Actuator (LED...).
 */
class SBMessageRequestResponse : public SBMessage {
public:

	char sbaddress[ADR_TYPE_SIZEOF];
	char value;

	/**
	 * Contructors
	 */
	SBMessageRequestResponse(const char *sbaddress):SBMessage(SBTResponseType(SBMsgReqType::request)) {
		memcpy(this->sbaddress, sbaddress, ADR_TYPE_SIZEOF);
	};
	SBMessageRequestResponse(const char *sbaddress, char value): SBMessageRequestResponse(sbaddress) {
		this->value= '0' + (value % 2);
	};

};



/**
 * Base MessageRequest for binary state sensor (switch...)
 * 2 states devices
 */
class SBBinaryStateBaseMessageReq : public SBMessage {
public:

	char sbaddress[ADR_TYPE_SIZEOF];
	char value;
	const char batDelimit='B';
	char batterylevel; // Must be '0' empty to '9' full

	char deviceType =SBDeviceType::sensor;
	char moduleType[MODULE_TYPE_SIZEOF];

	/**
	 * Contructors
	 */
	SBBinaryStateBaseMessageReq(const char messageType, const SBBinaryStateSensor * const device);


};

/**
 * Watchdog Message for binary state Sensor.
 * Message request :
 * Les capteurs se signalent par un message toutes les minutes, si pas de réponse, ils recommencent toutes les 2 secondes jusqu’à réponse correcte.
 */
class SBBinaryStateMessageWatchdogReq : public SBBinaryStateBaseMessageReq {
public:
	/**
	 * Contructors
	 */
	SBBinaryStateMessageWatchdogReq(const SBBinaryStateSensor * const device);
};

/**
 * Watchdog Response Message for binary state sensor.
 */
class SBBinaryStateMessageWatchdogResponse : public SBMessage {
public:

	char sbaddress[ADR_TYPE_SIZEOF];
	char value;

	char deviceType =SBDeviceType::sensor;
	char moduleType[MODULE_TYPE_SIZEOF];

	/**
	 * Contructors
	 */
	SBBinaryStateMessageWatchdogResponse(const char *sbaddress):SBMessage(SBTResponseType(SBMsgReqType::watchdog)) {
		memcpy(this->sbaddress, sbaddress, ADR_TYPE_SIZEOF);
	};
	SBBinaryStateMessageWatchdogResponse(const char *sbaddress, char value): SBBinaryStateMessageWatchdogResponse(sbaddress) {
		this->value= '0' + (value % 2);
	};

};


/**
 * Data Message for binary state  sensor.
 * Les capteurs envoient leur valeurs si changement d’état. Ils essayent pendant 2 seconde jusqu’à obtenir un acquittement, sinon attendent le prochain changement d’état
 */
class SBBinaryStateMessageDataReq : public SBBinaryStateBaseMessageReq {
public:
	/**
	 * Contructors
	 */
	SBBinaryStateMessageDataReq(const SBBinaryStateSensor * const device);
};

#endif // __SBMESSAGE_H_
