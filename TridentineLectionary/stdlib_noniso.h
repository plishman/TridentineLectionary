#pragma once
/*
stdlib_noniso.h - nonstandard (but usefull) conversion functions
Copyright (c) 2014 Ivan Grokhotkov. All rights reserved.
This file is part of the esp8266 core for Arduino environment.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef STDLIB_NONISO_H
#define STDLIB_NONISO_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__
	int _atoi(const char *s);

	long _atol(const char* s);

	double _atof(const char* s);

	char* _itoa(int val, char *s, int radix);

	char* _ltoa(long val, char *s, int radix);
#endif

	char* utoa(unsigned int val, char *s, int radix);

#ifdef __linux__
	char* _ultoa(unsigned long val, char *s, int radix);
#endif

	char* dtostrf(double val, signed char width, unsigned char prec, char *s);

	int digitsBe4Decimal(double number);

#ifdef __cplusplus
} // extern "C"
#endif

#endif