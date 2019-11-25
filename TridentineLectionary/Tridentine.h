#pragma once
#ifndef TRIDENTINE_H
#define TRIDENTINE_H

#include "TimeLib.h"
#include "WString.h"  
#include "yml.h"
#include "Ordinalizer.h"

const int BEGIN_EPOCH = 1970; // 1900 for 64-bit time64_t, sometimes 1970 (may be on embedded system)

//python-compatible values for weekdays (use with weekday() function)
#define PY_MON 0
#define PY_TUE 1
#define PY_WED 2
#define PY_THU 3
#define PY_FRI 4
#define PY_SAT 5
#define PY_SUN 6

#define SEASON_ORDINARY			   0
#define SEASON_ADVENT			   1
#define SEASON_CHRISTMAS		   2
#define SEASON_EPIPHANY			   3
#define SEASON_SEPTUAGESIMA		   4 // pre-lent season in which Violet colours are worn on Sunday but not weekdays: see https://catholicherald.co.uk/commentandblogs/2018/01/28/what-is-septuagesima/
#define SEASON_LENT				   5
#define SEASON_EASTER			   6
#define SEASON_PENTECOST		   7
#define SEASON_AFTER_PENTECOST	   8

struct Tr_Calendar_Day {
	time64_t datetime;
	String DayofWeek;
	String Class;
	String Colour;
	String Mass;
	String Commemoration;
	bool HolyDayOfObligation;
};

class Tridentine {
public:
	static const char* const Feasts[68];
	static time64_t date(int day, int month, int year);
	static int date_difference(time64_t date1, time64_t date2);
	static void print_date(time64_t datetime);
	static int year(time64_t date);
	static int liturgical_year(time64_t datetime);
	static bool isleap(int year);
	static bool issameday(time64_t date1, time64_t date2);
	static time64_t weekday_after(int weekdayAfter, time64_t date);
	static bool sunday(time64_t date);
	static time64_t sunday_after(time64_t date);
	static int dayofmonth(time64_t date);
	static bool firstday(uint8_t day, time64_t datetime);
	static time64_t start_date(int year);
	static time64_t end_date(int year);
	static time64_t first_advent_sunday(int year);
	static time64_t nativity(int year);
	static time64_t weekday_before(int weekdayBefore, time64_t date);
	static time64_t sunday_before(time64_t date);
	static int weekday(time64_t date, bool bUsePythonWeekdayFormat = true);
	static time64_t computus(int year);
	static time64_t GaudeteSunday(int year);
	static time64_t AdventEmbertide(int year, int day);
	static time64_t SundayWithinTheOctaveOfXmas(int year);
	static time64_t HolyName(int year);
	static time64_t HolyFamily(int year);
	static time64_t PloughMonday(int year);
	static time64_t AshWednesday(int year);
	static time64_t LentenEmbertide(int year, int day);
	static time64_t Quinquagesima(int year);
	static time64_t FatThursday(int year);
	static time64_t ShroveMonday(int year);
	static time64_t MardiGras(int year);
	static time64_t Sexagesima(int year);
	static time64_t Septuagesima(int year);
	static time64_t StMatthias(int year);
	static time64_t StGabrielOfOurLadyOfSorrows(int year);
	static time64_t LaetareSunday(int year);
	static time64_t PassionSunday(int year);
	static time64_t SevenSorrows(int year);
	static time64_t LadyDay(int year);
	static time64_t StJoseph(int year);
	static time64_t PalmSunday(int year);
	static time64_t MondayOfHolyWeek(int year);
	static time64_t TuesdayOfHolyWeek(int year);
	static time64_t SpyWednesday(int year);
	static time64_t MaundyThursday(int year);
	static time64_t GoodFriday(int year);
	static time64_t HolySaturday(int year);
	static time64_t Easter(int year);
	static time64_t EasterMonday(int year);
	static time64_t EasterTuesday(int year);
	static time64_t EasterWednesday(int year);
	static time64_t EasterThursday(int year);
	static time64_t EasterFriday(int year);
	static time64_t EasterSaturday(int year);
	static time64_t QuasimodoSunday(int year);
	static time64_t MisericordiaSunday(int year);
	static time64_t JubilateSunday(int year);
	static time64_t CantateSunday(int year);
	static time64_t MajorRogation(int year);
	static time64_t AscensionVigil(int year);
	static time64_t Ascension(int year);
	static time64_t SundayAfterAscension(int year);
	static time64_t MinorRogation(int year, int day);
	static time64_t PentecostVigil(int year);
	static time64_t Pentecost(int year);
	static time64_t PentecostMonday(int year);
	static time64_t PentecostTuesday(int year);
	static time64_t WhitEmbertide(int year, int day);
	static time64_t ThursdayInPentecostWeek(int year);
	static time64_t TrinitySunday(int year);
	static time64_t CorpusChristi(int year);
	static time64_t SacredHeart(int year);
	static time64_t PetersPence(int year);
	static time64_t MichaelmasEmbertide(int year, int day);
	static time64_t AllSouls(int year);
	static time64_t ChristTheKing(int year);
	static uint8_t Season(time64_t datetime);
	static time64_t Season_beginning(uint8_t season, time64_t datetime);	static uint8_t Season_Week(time64_t datetime, uint8_t season);
	static void ColourAndClass(time64_t datetime, uint8_t& col, uint8_t& cls);
	static void ColourAndClass(time64_t datetime, bool doRogations, uint8_t& col, uint8_t& cls);
	static bool IsEmberDay(time64_t datetime);
	static void GetMassAndCommFromTrFixedFeast(Tr_Fixed_Feast& trff, String& Mass, String& Commemoration, uint8_t season, uint8_t day_class);
	static void HandleVotiveMasses(time64_t datetime, uint8_t& cls, uint8_t& col, String& Mass, String& Commemoration);
	static void HandleVotiveMasses(time64_t datetime, bool& is_votive, uint8_t& cls, uint8_t& col, String& Mass, String& Commemoration);
	static void get(time64_t datetime, Tr_Calendar_Day& td, bool doRogations = true);
	static void GetMoveableFeast(time64_t datetime, bool doRogations, bool& is_feast, uint8_t& cls, uint8_t& col, bool& hdo, bool& feast_lord, bool& bCommemorationOnly, String& Mass, String& Commemoration);
	static void GetFixedFeast(time64_t datetime, bool& is_feast, uint8_t& cls, uint8_t& col, bool& hdo, bool& feast_lord, bool& immaculate_conception, bool& bCommemorationOnly, String& Mass, String& Commemoration);
	static void GetVotiveMass(time64_t datetime, bool& is_votive, uint8_t& cls, uint8_t& col, String& Mass, String& Commemoration);
	static void GetDay(time64_t datetime, String& Mass);

	
};
#endif