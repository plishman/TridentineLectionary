// TridentineLectionary.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
//#include <iostream>
#include <stdio.h>
#include "Tridentine.h"

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



int main()
{
    //std::cout << "Hello World!\n"; 
/*
	for (int y = 2000; y <= 2020; y++) {
		printf("%d\t", y);
		for (int m = 1; m <= 12; m++) {
			printf("%c ", dominical(m, y, 1));
		}
		printf("\n");
	}
*/

	FILE* fp; // empty output.csv if it exists
	fp = fopen("output.csv", "w");
	fclose(fp);

#if (false)
	Tridentine::get(Tridentine::date(20, 3, 2017));
	Tridentine::get2(Tridentine::date(20, 3, 2017), true);
	/*
	Tridentine::get(Tridentine::date(29, 12, 2019));	
	Tridentine::get(Tridentine::date(18, 10, 2015));
	Tridentine::get(Tridentine::date(25, 12, 2019));
	Tridentine::get(Tridentine::date(25, 12, 2019) + 3600 * 6);
	Tridentine::get(Tridentine::date(25, 12, 2019) + 3600 * 12);
	Tridentine::get(Tridentine::date(25, 12, 2019) + 3600 * 13);
*/
#else
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
#endif
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


//////// need to solve the liturgical year/ year -1 prob.
//////// solved it in Temporale.cpp using year+1 for easter year,
//////// and letting year = year-1 at all other times.