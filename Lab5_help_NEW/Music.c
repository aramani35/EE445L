#include "DAC.h"
#include "Music.h"

const unsigned short Wave[64] = {
	2048,2224,2399,2571,2737,2897,3048,3190,3321,3439,3545,3635,3711,3770,3813,3839,3848,3839,3813,3770,
	3711,3635,3545,3439,3321,3190,3048,2897,2737,2571,2399,2224,2048,1872,1697,1525,1359,1199,1048,906,775,
	657,551,461,385,326,283,257,248,257,283,326,385,461,551,657,775,906,1048,1199,1359,1525,1697,1872
};

const unsigned short oboe[64] = {
	2126,2164,2238,2550,3356,3496,2550,1510,1322,1322,1406,1462,1538,1690,2078,2268,2418,2664,2930,
	3090,2854,3176,2740,2172,1416,1038,896,980,1132,1368,1604,1984,2126,2164,2238,2550,3356,3496,2550,
	1510,1322,1322,1406,1462,1538,1690,2078,2268,2418,2664,2930,3090,2854,3176,2740,2172,1416,1038,
	896,980,1132,1368,1604,1984
};

const unsigned short trumpet[64] = {
	2014,2176,2312,2388,2134,1578,606,198,1578,3020,2952,2346,2134,2074,2168,2124,2022,2030,2090,2124,
	2022,2022,2116,2226,2168,2150,2158,2210,2176,2098,2030,2090,2014,2176,2312,2388,2134,1578,606,198,
	1578,3020,2952,2346,2134,2074,2168,2124,2022,2030,2090,2124,2022,2022,2116,2226,2168,2150,2158,
	2210,2176,2098,2030,2090
};

const unsigned short flute[64] = {
	2014,2504,2748,3096,3396,3594,3650,3594,3350,3124,2766,2438,2184,2014,1826,1780,1666,1694,1620,1554,
	1488,1348,1196,1102,1018,952,990,1066,1178,1318,1516,1752,2014,2504,2748,3096,3396,3594,3650,3594,
	3350,3124,2766,2438,2184,2014,1826,1780,1666,1694,1620,1554,1488,1348,1196,1102,1018,952,990,1066,
	1178,1318,1516,1752
};

const unsigned short bassoon[64] = {
	2136,2338,2350,2322,2260,2226,2204,2152,2064,1970,1926,1974,2164,2686,3474,3726,3150,2062,1076,618,
	660,944,1252,1614,2076,2540,2840,2922,2750,2402,2010,1638,1316,1064,992,1188,1608,2110,2496,2646,
	2466,2098,1790,1652,1652,1700,1724,1722,1798,1922,2012,2046,2092,2184,2354,2448,2372,2266,2196,
	2204,2218,2152,2054,2006
};


/**
 * Get the current pitch for an instrument with the specified voice index.
 */
float factor;
uint16_t Sound_Config(uint32_t index_of_sound, uint32_t timeSinceNewNote) {
	Instrument currentVoice = Song_CurrentNote()->voice;
	factor = Instrument_EnvelopeMultiplier(timeSinceNewNote);
	switch(currentVoice) {
		case Flute: return flute[index_of_sound % 64]*factor/4096;
		case Bassoon: return bassoon[index_of_sound % 64];
		case Oboe: return oboe[index_of_sound % 64];
		case Trumpet: return trumpet[index_of_sound % 64];
	}
	return Wave[index_of_sound % 64];
}

float scale;
float Instrument_EnvelopeMultiplier(uint32_t timeIndex) {
	if (timeIndex < 1000) scale = 4096* (float)timeIndex / 1000;
	else if (timeIndex < 50000) scale = 4096 - 4096*(float)(timeIndex) / 50000;
	else scale = 0;
	return 4096;
}

uint32_t noteIndex = 0;
uint32_t beatIndex = 0;
Song currentSong;

void Song_PlayInit(Song song) {
	currentSong = song;
	noteIndex = 0;
	beatIndex = 0;
}

/**
 * Get the current output voltage for the specified beat index for this song.
 * The beat index is the index of which note in the song to find a pitch for.
 */
Note* Song_CurrentNote() {
	return &currentSong.notes[noteIndex];
}

void Song_PlayHandler(void){
	Note currentNote = *Song_CurrentNote();
	beatIndex += 1;
	if(beatIndex >= currentNote.duration * 2) {
		noteIndex += 1;
		beatIndex = 0;
	}
}

Note song_of_storms[] = {
	{D, Eighth, Flute},
	{F, Eighth, Flute},
	{D1, Half, Flute},
	{D, Eighth, Flute},
	{F, Eighth, Flute},
	{D1, Half, Flute},
	{E1, QuarterAndEighth, Flute},
	{F1, Eighth, Flute},
	{E1, Eighth, Flute},
	{F1, Eighth, Flute},
	{E1, Eighth, Flute},
	{C, Eighth, Flute},
	{A, Half, Flute},
	{A, Quarter, Flute},
	{D, Quarter, Flute},
	{F, Eighth, Flute},
	{G, Eighth, Flute},
	{A, HalfAndQuarter, Flute},
	{A, Quarter, Flute},
	{D, Quarter, Flute},
	{F, Eighth, Flute},
	{G, Eighth, Flute},
	{E, HalfAndQuarter, Flute},
	{D, Eighth, Flute},
	{F, Eighth, Flute},
	{D1, Half, Flute},
	{D, Eighth, Flute},
	{F, Eighth, Flute},
	{D1, Half, Flute},
	{E1, QuarterAndEighth, Flute},
	{F1, Eighth, Flute},
	{E1, Eighth, Flute},
	{F1, Eighth, Flute},
	{E1, Eighth, Flute},
	{C, Eighth, Flute},
	{A, Half, Flute},
	{A, Quarter, Flute},
	{D, Quarter, Flute},
	{F, Eighth, Flute},
	{G, Eighth, Flute},
	{A, Half, Flute},
	{A, Quarter, Flute},
	{D, HalfAndQuarter, Flute},
	
	{0, Whole, Flute}
};

Note great_fairy_fountain[] = {
	{C0, Quarter, Flute},
	{E, Quarter, Flute},
	{G, Quarter, Flute},
	{BF, Quarter, Flute},
	{D1, Quarter, Flute},
	{F1, Quarter, Flute},
	
	{A1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{G1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{GF1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{G1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{G1, Eighth, Flute},
	{C, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	
	{F1, Eighth, Flute},
	{C, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	
	{E1, Eighth, Flute},
	{C, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	
	{F1, Eighth, Flute},
	{C, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	
	{F1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	{E, Eighth, Flute},
	
	{E1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	{E, Eighth, Flute},
	
	{EF1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	{E, Eighth, Flute},
	
	{E1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	{E, Eighth, Flute},
	
	{E1, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	{D, Eighth, Flute},
	
	{D1, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	{D, Eighth, Flute},
	
	{DF1, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	{D, Eighth, Flute},
	
	{D1, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	{D, Eighth, Flute},
	
	{A1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{G1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{GF1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{G1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{BF1, Eighth, Flute},
	{EF1, Eighth, Flute},
	{C, Eighth, Flute},
	{GF, Eighth, Flute},
	
	{A1, Eighth, Flute},
	{EF1, Eighth, Flute},
	{C, Eighth, Flute},
	{GF, Eighth, Flute},
	
	{AF1, Eighth, Flute},
	{EF1, Eighth, Flute},
	{C, Eighth, Flute},
	{GF, Eighth, Flute},
	
	{A1, Eighth, Flute},
	{EF1, Eighth, Flute},
	{C, Eighth, Flute},
	{GF, Eighth, Flute},
	
	{C1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{BF1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{A1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{BF1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{A1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{E, Eighth, Flute},
	
	{G1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{E, Eighth, Flute},
	
	{F1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{E, Eighth, Flute},
	
	{E1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{E, Eighth, Flute},
	
	{A1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{G1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{GF1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{G1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{G1, Eighth, Flute},
	{C, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	
	{F1, Eighth, Flute},
	{C, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	
	{E1, Eighth, Flute},
	{C, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	
	{F1, Eighth, Flute},
	{C, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	
	{F1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	{E, Eighth, Flute},
	
	{E1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	{E, Eighth, Flute},
	
	{EF1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	{E, Eighth, Flute},
	
	{E1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	{E, Eighth, Flute},
	
	{E1, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	{D, Eighth, Flute},
	
	{D1, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	{D, Eighth, Flute},
	
	{DF1, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	{D, Eighth, Flute},
	
	{D1, Eighth, Flute},
	{A, Eighth, Flute},
	{F, Eighth, Flute},
	{D, Eighth, Flute},
	
	{A1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{G1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{GF1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{G1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{BF1, Eighth, Flute},
	{EF1, Eighth, Flute},
	{C, Eighth, Flute},
	{GF, Eighth, Flute},
	
	{A1, Eighth, Flute},
	{EF1, Eighth, Flute},
	{C, Eighth, Flute},
	{GF, Eighth, Flute},
	
	{AF1, Eighth, Flute},
	{EF1, Eighth, Flute},
	{C, Eighth, Flute},
	{GF, Eighth, Flute},
	
	{A1, Eighth, Flute},
	{EF1, Eighth, Flute},
	{C, Eighth, Flute},
	{GF, Eighth, Flute},
	
	{C1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{BF1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{A1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{BF1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{G, Eighth, Flute},
	
	{A1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{E, Eighth, Flute},
	
	{G1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{E, Eighth, Flute},
	
	{F1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{E, Eighth, Flute},
	
	{E1, Eighth, Flute},
	{D1, Eighth, Flute},
	{BF, Eighth, Flute},
	{E, Eighth, Flute},
	
	{0, Whole, Flute}
};

Note wholeNotesTest[] = {
//	{A, Whole, Flute},
//	{B, Whole, Flute},
//	{C, Whole, Flute},
//	{D1, Whole, Flute},
//	{E1, Whole, Flute},
//	{F1, Whole, Flute},
//	{G1, Whole, Flute},
	
	{C_2, 2000, Flute},
	
	{0, Whole, Flute}
};
