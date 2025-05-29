// data.c
// project: weedmate

#include <weedmate/common.h>

int currentCityIndex = 3;



city_s cities[CITY_COUNT] = {
	{ "Detroit", 			0.65f },
	{ "San Francisco",		1.95f },
	{ "Minneapolis",		1.30f },
	{ "Denver",				1.02f }
};



strain_s strains[STRAIN_COUNT] = {
	{ "White Death",			13 },
	{ "Purple Plutonium OG",	12 },
	{ "Dankmestic Abuse",		24 },
	{ "Fentanyl Kush",			48 },
	{ "Blunt Trauma",			10 },
	{ "Petroleum #42",			35 },
	{ "Ganjaffi",				23 },
	{ "Dental Prescription",	 9 },
	{ "Hebrew Kush",			 6 },
	{ "Rohypno-Chronic",		30 },
	{ "Holy Shit OG",			 8 },
	{ "George W Kush",			 8 },
	{ "Bad Dragon",				17 },
	{ "Carpet Muncher",			11 },
	{ "Freedom Highve",			22 }
};
