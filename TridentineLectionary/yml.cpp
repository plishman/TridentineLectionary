#include "yml.h"
#include <stdio.h>

#ifdef _WIN32
#include <direct.h>
#endif

void Yml::SetConfig(String lang, String yml_filename, String sanct_filename) {
	Yml::lang = lang;
	Yml::yml_filename = yml_filename;
	Yml::sanct_filename = sanct_filename;
}

String Yml::lang = "en";

#ifdef _WIN32
String Yml::yml_filename = ".\\en-1962.yml";
String Yml::sanct_filename = ".\\en-1962.txt";
#else
String Yml::yml_filename = "./en-1962.yml";
String Yml::sanct_filename = "./en-1962.txt";
#endif

String Yml::getdate(time64_t t) {
	tmElements_t ts;
	breakTime(t, ts);

	String date_format = get("date");

	date_format.replace("%{d}", String(ts.Day));
	date_format.replace("%{dd}", ts.Day > 9 ? String(ts.Day) : "0" + String(ts.Day));

	date_format.replace("%{m}", String(ts.Month));
	date_format.replace("%{mm}", ts.Month > 9 ? String(ts.Month) : "0" + String(ts.Month));
	date_format.replace("%{mmm}", get("month." + String(ts.Month)));

	String yy = tmYearToCalendar(ts.Year) % 100 < 10 ? "0" + String(tmYearToCalendar(ts.Year) % 100) : String(tmYearToCalendar(ts.Year) % 100);

	date_format.replace("%{yy}", yy);
	date_format.replace("%{yyyy}", String(tmYearToCalendar(ts.Year)));

	date_format.replace("%{day}", get("weekday." + String(ts.Wday - 1)));

	return date_format;
}

//void Yml::SetConfig(String filename, String lang) {
//	_yml_filename = filename;
//	_lang = lang;
//}

String Yml::get(String I18nPath) {
	bool bError = false;
	return get(I18nPath, bError);
}

String Yml::get(String I18nPath, bool& bError) {
#ifndef __AVR__
	//printf("Get() token: %s\n", I18nPath.c_str());
#endif

	bError = true;

	if (I18nPath == "") return "";	// no output if no path

#ifdef __AVR__
	//DEBUG_PRT.println("I18n::get()");

	if (!configparams.have_config) {
		DEBUG_PRT.println(F("I18n::get(): No config"));
		return "";
	}

	if (_suppress_output == true) return "";

	//	if (!initializeSD()) return String("");
#else
	char* filestr;
#endif

	I18nPath = Yml::lang + String(".") + I18nPath;

#ifdef __AVR__
	String I18nFilename = "locales/" + String(I18n_LANGUAGES[_locale]) + ".yml";
	File file = openFile(I18nFilename, FILE_READ);
	if (!file) return "";
#else
	//char filepath[2048];
	//_getcwd(filepath, 2048);
	//printf("cwd=[%s]\n", filepath);
	//_yml_filename = String(filepath) + "\\..\\" + _yml_filename;
	//printf("fsp=[%s]\n", _yml_filename.c_str());
	FILE* fpi = fopen(Yml::yml_filename.c_str(), "r");
	if (fpi == NULL) return "";
	char buf[1024];
#endif

	bool bTokenMatched = false;
	String lookingforToken;
	String readToken;
	String readData;
	String readLine;
	String I18nCurrentPath;

	int l;

	int currenttokendepth = 0;
	int readtokendepth = 0;

	int numtokensinI18nPath = 1;
	l = I18nPath.length();
	for (int i = 0; i < l; i++) {
		if (I18nPath.charAt(i) == '.') numtokensinI18nPath++;
	}

	int numtokensinI18nCurrentPath = 0;

	int tokenindentwidth = 0;
	bool bFirstPass = true;

	do {
#ifdef __AVR__
#else
		filestr = fgets(buf, 1024, fpi);
		if (filestr == NULL) continue; // EOF: if at end, drop out of loop and check if anything was found
#endif

#ifdef __AVR__
		readLine = this->readLine(file);
		if (readLine == "") continue;
#else
		readLine = String(buf);
#endif

		readToken = readLine.substring(0, readLine.indexOf(":"));

		readtokendepth = readToken.lastIndexOf(" ") + 1;
		readToken.trim();

		l = readToken.length() - 1;

		if ((readToken.charAt(0) == '\'') && (readToken.charAt(l) == (const char)'\'')) {	// strip enclosing single quotes from token, if present
			readToken = readToken.substring(1, l);
		}

		if (bFirstPass == true && readtokendepth > 0) {
			tokenindentwidth = readtokendepth;		// calculate the width in characters of the indents (yaml files are structured by the indentation)
			bFirstPass = false;
		}

		if (readtokendepth == 0) {
			I18nCurrentPath = readToken;
			numtokensinI18nCurrentPath = 1;
		}

		if (currenttokendepth < readtokendepth) {
			I18nCurrentPath += "." + readToken;	// one level deeper, add on new token to path
			numtokensinI18nCurrentPath++;
		}

		if ((currenttokendepth != 0) && (currenttokendepth == readtokendepth)) { // same level, replace last token with read token
			I18nCurrentPath = I18nCurrentPath.substring(0, I18nCurrentPath.lastIndexOf("."));		// remove last field,
			I18nCurrentPath += "." + readToken;														// and replace it with the token just read
		}

		if ((readtokendepth != 0) && (currenttokendepth > readtokendepth)) { // most complicated case: remove number of tokens indicated by change in indentation, and replace end token
			for (int i = readtokendepth; i <= currenttokendepth; i += tokenindentwidth) {
				I18nCurrentPath = I18nCurrentPath.substring(0, I18nCurrentPath.lastIndexOf("."));	// remove last field,
				numtokensinI18nCurrentPath--;
			}
			I18nCurrentPath += "." + readToken;														// and replace it with the token just read
			numtokensinI18nCurrentPath++;
		}

		currenttokendepth = readtokendepth;

		//if ((I18nPath.indexOf(I18nCurrentPath) == 0) && numtokensinI18nCurrentPath == numtokensinI18nPath) { // found our key
		if (I18nPath == I18nCurrentPath) { // found our key
			readData = readLine.substring(readLine.indexOf(":") + 1);
			readData.trim();
			bTokenMatched = true;
		}
	}
#ifdef __AVR__
	while (file.available() && !bTokenMatched); // to loop, File needs to have returned data and token remained unmatched
#else
	while (filestr != NULL && !bTokenMatched); // to loop, File needs to have returned data and token remained unmatched
#endif

	if (!bTokenMatched) { // reached end of file without finding the token
#ifdef __AVR__
		DEBUG_PRT.print(F("token ")); DEBUG_PRT.print(lookingforToken); DEBUG_PRT.println(F(" not found (EOF)"));
		closeFile(file);
#else
		printf("token "); 
		printf("%s ", lookingforToken.c_str()); 
		printf("not found (EOF)\n");
		fclose(fpi);
#endif
		return String("");
	}

#ifdef __AVR__
	closeFile(file);
#else
	fclose(fpi);
#endif

	l = readData.length() - 1;

	if ((readData.charAt(0) == '\'') && (readData.charAt(l) == (const char)'\'')) {	// strip enclosing single quotes
		readData = readData.substring(1, l);
	}

	_callcount++;

	//printf("Found: read data: %s\n", readData.c_str());
	bError = false;
	return readData;
}



int Yml::dayofmonth(time64_t datetime) {
	::tmElements_t ts;						// for arduino
	::breakTime(datetime, ts);
	return ts.Day;
}

bool Yml::get_fixed_feast(time64_t date, Tr_Fixed_Feast& feast) {
	feast.IsFeast = false;
	feast.Class = 4;
	feast.Holy_Day_Of_Obligation = false;
	feast.Feast_Of_The_Lord = false;
	feast.ImmaculateConception = false;
	feast.Lectionary = 0;
	feast.Colour = TR_LIT_COLOUR_UNSET;
	feast.Mass = "";
	feast.bMassIsCommemoration = false;
	feast.Commemoration = "";
	feast.bHasCommemoration = false;


#ifdef __AVR__
	if (_I18n == NULL) {
		DEBUG_PRT.println(F("Sanctorale::get(): _I18n is null"));
		return false;
	}

	if (!(_I18n->configparams.have_config)) {
		DEBUG_PRT.println(F("Sanctorale::get(): No config"));
		return false;
	}
#else
	char buf[1024];
	char* filestr;
#endif

	String readtoken;
	String readtokendata;

	int m = ::month(date);
	int d = dayofmonth(date);
	int hour = ::hour(date);

	String month_token = "= " + String(m);
	String readLine = "";
	String readLine2 = "";

#ifdef __AVR__
	String I18nFilename = _I18n->configparams.sanctorale_filename; //"data/" + String(_I18n->I18n_SANCTORALE[_locale]);

	File file = _I18n->openFile(I18nFilename, FILE_READ);
	if (!file) {
		DEBUG_PRT.print(F("Sanctorale::get() couldn't open file ")); DEBUG_PRT.println(I18nFilename);
		return false;
	}
#else
	FILE* fpi = fopen(Yml::sanct_filename.c_str(), "r");
	if (fpi == NULL) return false;
#endif

	// scan for "= <month_number>"
	bool bFound = false;
	do {
#ifdef __AVR__
		readLine = _I18n->readLine(file);
		if (readLine == "") continue;
#else
		filestr = fgets(buf, 1024, fpi);
		if (filestr == NULL) continue; // EOF: if at end, drop out of loop and check if anything was found
		readLine = String(buf);
#endif

		if (readLine.indexOf(month_token) == 0) { // try to match month number, eg. =11 for November
			bFound = true;
		}
#ifdef __AVR__
	} while (!bFound && file.available());
#else
	} while (!bFound && !(feof(fpi)));
#endif

	if (!bFound) return false; // got to the end of the file without finding it

						   // scan for string starting with the same number as the day of the month
	bFound = false;
	bool bEndOfMonth = false;
	bool bGotLine2 = false;
	String daynumber = String(d) + " "; // day of month number starts the line, and is followed by a space
	String daynumber_with_hour = String(d) + "_" + String(hour) + " ";
	do {
#ifdef __AVR__
		readLine = _I18n->readLine(file);
		//if (readLine == "") continue;
		if (readLine == "" || readLine.indexOf("#") == 0) continue; // lines which start with a # are comments, and are ignored

#else
		filestr = fgets(buf, 1024, fpi);
		if (filestr == NULL) continue; // EOF: if at end, drop out of loop and check if anything was found
		readLine = String(buf);
		if (readLine == "" || readLine.indexOf("#") == 0) continue; // lines which start with a # are comments, and are ignored
#endif

		if (readLine.indexOf(daynumber_with_hour) == 0) { // got a day number with an hour (more than one Mass this day - eg. Christmas Day)
			bFound = true;
		}
		else {
			if (readLine.indexOf(daynumber) == 0) {	// try to match the day of month
				bFound = true;
			}
		}

		if (readLine == "" || readLine.indexOf("=") == 0) { // scanned past end of month
			bEndOfMonth = true;
		}
#ifdef __AVR__
	} while (!bFound && !bEndOfMonth && file.available());
#else
	} while (!bFound && !bEndOfMonth && !feof(fpi));
#endif

	if (!bFound || bEndOfMonth) {
#ifdef __AVR__
		_I18n->closeFile(file);
#else
		fclose(fpi);
#endif
		return false; // got to the end of the file without finding it
	}

	// now read one more line, and if it matches the same day then it will be a Commemoration
#ifdef __AVR__
	if (file.available) {
		readLine2 = _I18n->readLine(file);
	}
#else
	if (!feof(fpi)) {
		filestr = fgets(buf, 1024, fpi);
		readLine2 = String(buf);
	}
#endif

	if (readLine2.indexOf(daynumber_with_hour) == 0) { // match day number and hour for commemoration if present
		bGotLine2 = true;
	}
	else {
		if (readLine2.indexOf(daynumber) == 0) {	// otherwise match day number only for commemoration
			bGotLine2 = true;
		}
	}

#ifdef __AVR__
	_I18n->closeFile(file);
#else
	fclose(fpi);
#endif
	
	// scan from end of day number up to the first : char to get the flags
	String monthdayandflags = readLine.substring(0, readLine.indexOf(":"));

	bool bIsSanctorale = false;

	if (monthdayandflags.indexOf("o") != -1) {	// holy day of obligation
		feast.Holy_Day_Of_Obligation = true;
	}

	if (monthdayandflags.indexOf("f") != -1) {	// holy day of obligation
		feast.Feast_Of_The_Lord = true;
	}

	if (monthdayandflags.indexOf("i") != -1) {	// holy day of obligation
		feast.ImmaculateConception = true;
	}

	if (monthdayandflags.indexOf("c") != -1) {	// commemoration
		feast.bMassIsCommemoration = true; // the first line read that matched the day and month was a commemoration
	}
	else {
		feast.Colour = TR_LIT_COLOUR_WHITE;
	}

	if (monthdayandflags.indexOf(" 1 ") != -1) {// class I feast (class numbers must have space 
		feast.Class = 1;						// chars either side to distinguish them from 
	}											// lectionary index numbers (of the form"L102" etc.)

	if (monthdayandflags.indexOf(" 2 ") != -1) {// class II feast
		feast.Class = 2;						
	}											

	if (monthdayandflags.indexOf(" 3 ") != -1) {// class III feast
		feast.Class = 3;						
	}											// is a class IV feast or ferial day if not specified

	if (monthdayandflags.indexOf("R") != -1) {
		feast.Colour = TR_LIT_COLOUR_RED;
	}

	if (monthdayandflags.indexOf("G") != -1) {
		feast.Colour = TR_LIT_COLOUR_GREEN;
	}

	if (monthdayandflags.indexOf("W") != -1) {
		feast.Colour = TR_LIT_COLOUR_WHITE;
	}

	if (monthdayandflags.indexOf("V") != -1) {
		feast.Colour = TR_LIT_COLOUR_VIOLET;
	}

	if (monthdayandflags.indexOf("P") != -1) {
		feast.Colour = TR_LIT_COLOUR_ROSE;
	}

	feast.Mass = readLine.substring(readLine.indexOf(":") + 1);
	feast.Mass.trim();

	feast.Lectionary = setLectionaryNumber(monthdayandflags);

	feast.IsFeast = true;

	if (bGotLine2) {
		feast.Commemoration = readLine2.substring(readLine2.indexOf(":") + 1);
		feast.Commemoration.trim();
		feast.bHasCommemoration = true;
		// won't check second line to see if it has a 'c' flag - second line will always be a commemoration (if present)
	}

	return true;
}

uint16_t Yml::setLectionaryNumber(String s) {
	uint16_t Lectionary = 0;
	String digits = "0123456789";
	String n = "";

	// look for "L" followed immediately by an integer giving the lectionary number, e.g. L697
	int pos = s.indexOf("L");
	if (pos != -1) {
		pos++;
		while (pos < s.length() && digits.indexOf(s.charAt(pos)) != -1) {
			n += s.charAt(pos++);
		}

		Lectionary = n.toInt();
	}
	return Lectionary;
}
