#ifndef _ORDINALIZER_H
#define _ORDINALIZER_H

#ifdef __AVR__
#include "RCGlobals.h"

#include "Arduino.h"
#include "DebugPort.h"
#else
#include "WString.h"
#include <stdint.h>
#endif 

#define ORDINAL_COUNT 8

class Ordinalizer {
public:
	static const String types[8];
	uint8_t _ordinal_type = 0;
	
	Ordinalizer();
	Ordinalizer(String type);
	void SetType(String type);
	String ordinalize(int number);
	String ord_french(int number);
	String ord_english(int number);
	String to_roman(unsigned int value);
};

#endif