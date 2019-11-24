#pragma once
#ifndef _YML_H
#define _YML_H
#include "TimeLib.h"
#include "WString.h"

#define TR_LIT_COLOUR_UNSET  0
#define TR_LIT_COLOUR_WHITE  1
#define TR_LIT_COLOUR_GREEN  2
#define TR_LIT_COLOUR_RED    3
#define TR_LIT_COLOUR_VIOLET 4
#define TR_LIT_COLOUR_ROSE   5
#define TR_LIT_COLOUR_BLACK  6
#define TR_LIT_COLOUR_GOLD   7


struct Tr_Fixed_Feast {
	bool IsFeast;
	uint16_t Lectionary;
	bool Holy_Day_Of_Obligation;
	bool Feast_Of_The_Lord;
	bool ImmaculateConception; // this is an exception in that it can override a sunday in Advent
	uint8_t Class;
	uint8_t Colour;
	String Mass;
	bool bMassIsCommemoration; // sometimes both the first and second lines read will be commemorations
	String Commemoration;
	bool bHasCommemoration;
};


class Yml {
public:
	String _yml_filename = ".\\en-1962.yml";
	String _lang = "en";
	int _callcount = 0;
	String getdate(time64_t t);
	void SetConfig(String filename, String lang);
	String get(String I18nPath);

	String _sanct_filename = ".\\en-1962.txt";
	int _Lectionary = 0;
	bool _hdo = false;
	int _rank = 4;
	int dayofmonth(time64_t datetime);
	bool get_fixed_feast(time64_t date, Tr_Fixed_Feast& feast);
	uint16_t setLectionaryNumber(String s);
};

#endif
