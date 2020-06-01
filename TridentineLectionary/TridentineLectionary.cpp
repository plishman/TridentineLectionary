// TridentineLectionary.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
//#include <iostream>
#include <stdio.h>
#include "Tridentine.h"
#include "yml.h"

// s==0 -> Julian calendar, otherwise Gregorian
char dominical(int m, int y, int s) {
	int leap;
	int a, b;
	leap = (s == 0 && y % 4 == 0) || (s != 0 && (y % 4 == 0 && y % 100 != 0 || y % 400 == 0));
	a = (y % 100) % 28;
	b = (s == 0)*(4 + (y % 700) / 100 + 2 * (a / 4) + 6 * ((!leap)*(1 + (a % 4)) + (leap)*((9 + m) / 12))) % 7 +
		(s != 0)*(2 * (1 + (y % 400) / 100 + (a / 4)) + 6 * ((!leap)*(1 + (a % 4)) + (leap)*((9 + m) / 12))) % 7;
	b = (b == 0)*(b + 7) + (b != 0)*b;
	return (char)(64 + b);
}
// Dominical letter (year):  Letter assigned based on weekday of 1st day of year, A=1 .. G=7
// Dominical cycle (months): 
//							 ADDGBEGCFADF (1st Jan = A, So, if 1st Jan is Sat, 1st Feb = Sat + 3 days, =Tue (D, 3 letters after A in alphabet))
//							 JFMAMJJASOND (months)
//										  If dominical letter for *year* is the same as that for the first day of any month in that year, that month will have a Fri 13th



int main(int argc, char *argv[])
{
	printf("\nLatin Mass Calendar Generator\n\n");

	// check if 3 or 6 arguments provided (including argv[0], the filename/path of the program).
	if (argc != 3 && argc != 6) {
		printf("\nPurpose: Program outputs a liturgical calendar to the selected file for the user-selected liturgical year.\n\n");
		printf("Usage: %s <liturgical_year> <output_filename>\n", argv[0]);
		printf("\nLocalisation defaults: Language: en, Locale files:\n \".\\en-1962.yml\" (for moveable feasts) and \n \".\\en-1962.txt\" (for fixed feasts).\n");
		printf(" - Unless others are specified, these files should both be present in the same directory as the program for it to work properly\n");
		printf("\nIf you want to use other translations/locale files:\n");
		printf("Usage: %s <liturgical_year> <output_filename> <lang> <fixed_feasts_filename.txt> <moveable_feasts_filename.yml>\n", argv[0]);
		printf("\n\nError: Incorrect number of parameters\n");
		return 1;
	}


	// get and check year
	int year = 0;

	int n = sscanf(argv[1], "%d", &year);
	if (n != 1) {
		printf("\nPurpose: Program outputs a liturgical calendar to the selected file for the user-selected liturgical year.\n\n");
		printf("Usage: %s <liturgical_year> <output_filename>\n", argv[0]);
		printf("\nLocalisation defaults: Language: en, Locale files:\n \".\\en-1962.yml\" (for moveable feasts) and \n \".\\en-1962.txt\" (for fixed feasts).\n");
		printf(" - Unless others are specified, these files should both be present in the same directory as the program for it to work properly\n");
		printf("\nIf you want to use other translations/locale files:\n");
		printf("Usage: %s <liturgical_year> <output_filename> <lang> <fixed_feasts_filename.txt> <moveable_feasts_filename.yml>\n", argv[0]);
		printf("\n\nError: Couldn't read a valid number for the year\n");
		return 2;
	}

	FILE* fp;

	// set locale if provided
	String locale = "en";
#ifdef _WIN32
	String fixed_txt = ".\\en-1962.txt";
	String moveable_yml = ".\\en-1962.yml";
#else
	String fixed_txt = "./en-1962.txt";
	String moveable_yml = "./en-1962.yml";
#endif 

	if (argc == 6) {
		locale = argv[3];
		fixed_txt = argv[4];
		moveable_yml = argv[5];
	}
	else {
		printf("Localisation files not specified, using default (English) if available\n");
	}

	printf("Localisation settings: \n");
	printf(" locale = %s\n", locale.c_str());
	printf(" fixed feasts file (.txt file) set to %s\n", fixed_txt.c_str());
	printf(" moveable feasts file (.yml file) set to %s\n\n", moveable_yml.c_str());

	bool bError = false;
		
	fp = fopen(fixed_txt.c_str(), "r");
	if (fp == NULL) {
		bError = true;
		printf("\nError:\n txt/fixed feasts file %s not found or inaccessible\n", fixed_txt.c_str());
	}
	else {
		fclose(fp);
	}

	fp = fopen(moveable_yml.c_str(), "r");
	if (fp == NULL) {
		if (!bError) {
			printf("\nError:\n");
		}
		bError = true;
		printf(" yml/moveable feasts file %s not found or inaccessible\n", moveable_yml.c_str());
	}
	else {
		fclose(fp);
	}

	if (bError) {
		return 3;
	}

	Yml::SetConfig(locale, moveable_yml, fixed_txt);

	Yml i18n;
	bError = false;
	i18n.get("or", bError); // the translation for the word 'or' is used for localization in the epaper lectionary project, and is the last entry in the yml file. Therefore if it can be read, the file is likely parsable
	if (bError) {
		printf("Error reading key in yml file %s (did you put the locale files in the wrong order on the command line?)\n", moveable_yml.c_str());
		return 4;
	}

	// check if output file exists
	fp = fopen(argv[2], "r");
	if (fp != NULL) {
		printf("\nFile exists - Ok to overwrite file?[y/N] ");
		char r = ' ';
		int n = scanf("%c", &r);
		if (r != 'y' && r != 'Y') {
			printf(" Cancelled\n");
			fclose(fp);
			return 5;
		}
	}

	// empty output file if it exists
	fp = fopen(argv[2], "w");
	fclose(fp);

	
	// Output days of liturgical year
	Tr_Calendar_Day cd;

	time64_t start_datetime = Tridentine::Season_beginning(SEASON_ADVENT, Tridentine::date(1, 1, year - 1));
	time64_t end_datetime = Tridentine::Season_beginning(SEASON_ADVENT, Tridentine::date(1, 1, year));

	time64_t datetime = start_datetime;

	fp = fopen(argv[2], "ab");
	if (fp == NULL) {
		printf("\nError: Couldn't open output file %s for writing.\n", argv[2]);
		return 6;
	}

	String headers = "\"Liturgical Year\",\"Date\",\"Day of Week\",\"Class\",\"Obligatory\",\"Colour\",\"Mass\",\"Commemoration\"\n";
	fwrite(headers.c_str(), 1, headers.length(), fp);

	while (datetime < end_datetime) {
		Tridentine::get(datetime, cd);
		
		String yearanddate = String(year) + "," + String(::day(datetime)) + "-" + String(::month(datetime)) + "-" + String(::year(datetime));
		String hdo = cd.HolyDayOfObligation ? "+" : "";
		String liturgicalday = yearanddate + ",\"" + cd.DayofWeek + "\",\"" + cd.Class + "\",\"" + hdo + "\",\"" + cd.Colour + "\",\"" + cd.Mass + "\",\"" + cd.Commemoration + "\",\"" + cd.Filename1962 + "\"\n";

		//printf("%4d,%02d-%02d-%4d,\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n", year, ::day(datetime), ::month(datetime), ::year(datetime), DayofWeek.c_str(), Class.c_str(), Colour.c_str(), Mass.c_str(), Commemoration.c_str());
		fwrite(liturgicalday.c_str(), 1, liturgicalday.length(), fp);
		
		datetime += SECS_PER_DAY;
		printf(".");
	}

	printf("\nCalendar written to file %s\n", argv[2]);
	fclose(fp);
	return 0;

/*
	int daycounts[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	for (int year = 2019; year < 2020; year++) {
		for (int month = 1; month <= 12; month++) {
			int numdays = daycounts[month];
			if (month == 2 && Tridentine::isleap(year)) {
				numdays = 29;
			}
			for (int day = 1; day <= numdays; day++) {
				Tridentine::get(Tridentine::date(day, month, year), true);
			}
		}
	}
*/
}