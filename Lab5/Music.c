#include "Music.h"
#include "Switches.h"


//const uint16_t wave[32] = {
//  2048*2,2448*2,2832*2,3186*2,3496*2,3751*2,3940*2,4057*2,4095*2,4057*2,3940*2,
//  3751*2,3496*2,3186*2,2832*2,2448*2,2048*2,1648*2,1264*2,910*2,600*2,345*2,
//  156*2,39*2,0*2,39*2,156*2,345*2,600*2,910*2,1264*2,1648*2};

const unsigned char Sinwave[32] = {  
	8,9,11,12,13,14,14,15,15,15,15,
	14,13,12,11,9,8,7,5,4,3,2,
	2,1,1,1,2,2,3,4,5,7
};

 unsigned int HappyBdaySong[] = {
	262, 262, 294, 262,349,330,262, 262,294,262,392,349,262,262,523,440,349,330,294,466,466,440,349,392,349
 };
 
 const unsigned short Wave[32] = {  
  1024,1122,1215,1302,1378,1440,1486,1514,1524,1514,1486,
  1440,1378,1302,1215,1122,1024,926,833,746,670,608,
  562,534,524,534,562,608,670,746,833,926
};  

const unsigned short Horn[32] = {  	
  1063,1082,1119,1275,1678,1748,1275,755,661,661,703,	
  731,769,845,1039,1134,1209,1332,1465,1545,1427,1588,	
  1370,1086,708,519,448,490,566,684,802,992	
};  	

 const unsigned short flute32[32] = {  	
  1007,1252,1374,1548,1698,1797,1825,1797,1675,1562,1383,	
  1219,1092,1007,913,890,833,847,810,777,744,674,	
  598,551,509,476,495,533,589,659,758,876	
};  	
 
const unsigned short flute64[64] = {
	2014,2504,2748,3096,3396,3594,3650,3594,3350,3124,2766,2438,2184,2014,1826,1780,1666,1694,1620,1554,
	1488,1348,1196,1102,1018,952,990,1066,1178,1318,1516,1752,2014,2504,2748,3096,3396,3594,3650,3594,
	3350,3124,2766,2438,2184,2014,1826,1780,1666,1694,1620,1554,1488,1348,1196,1102,1018,952,990,1066,
	1178,1318,1516,1752
};
 
const unsigned short oboe64[64] = {
	2126,2164,2238,2550,3356,3496,2550,1510,1322,1322,1406,1462,1538,1690,2078,2268,2418,2664,2930,
	3090,2854,3176,2740,2172,1416,1038,896,980,1132,1368,1604,1984,2126,2164,2238,2550,3356,3496,2550,
	1510,1322,1322,1406,1462,1538,1690,2078,2268,2418,2664,2930,3090,2854,3176,2740,2172,1416,1038,
	896,980,1132,1368,1604,1984
};

const unsigned short trumpet64[64] = {
	2014,2176,2312,2388,2134,1578,606,198,1578,3020,2952,2346,2134,2074,2168,2124,2022,2030,2090,2124,
	2022,2022,2116,2226,2168,2150,2158,2210,2176,2098,2030,2090,2014,2176,2312,2388,2134,1578,606,198,
	1578,3020,2952,2346,2134,2074,2168,2124,2022,2030,2090,2124,2022,2022,2116,2226,2168,2150,2158,
	2210,2176,2098,2030,2090
};

const unsigned short Trumpet32[32] = {  	
  1007,1088,1156,1194,1067,789,303,99,789,1510,1476,	
  1173,1067,1037,1084,1062,1011,1015,1045,1062,1011,1011,	
  1058,1113,1084,1075,1079,1105,1088,1049,1015,1045	
};  	

const unsigned short bassoon64[64] = {
	2136,2338,2350,2322,2260,2226,2204,2152,2064,1970,1926,1974,2164,2686,3474,3726,3150,2062,1076,618,
	660,944,1252,1614,2076,2540,2840,2922,2750,2402,2010,1638,1316,1064,992,1188,1608,2110,2496,2646,
	2466,2098,1790,1652,1652,1700,1724,1722,1798,1922,2012,2046,2092,2184,2354,2448,2372,2266,2196,
	2204,2218,2152,2054,2006
};


// for pitch
uint16_t Instrument_CurrentVoltage(uint32_t voiceIndex) {
	Instrument currentVoice = Song_CurrentNote()->voice;
	switch(currentVoice) {
		case Flute: return flute64[voiceIndex % 64];
		case Bassoon: return bassoon64[voiceIndex % 64];
		case Oboe: return oboe64[voiceIndex % 64];
		case Trumpet: return trumpet64[voiceIndex % 64];
	}
	return Wave[voiceIndex % 64];
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
	return &currentSong.notes[noteIndex];	// Accesses current from struct
}

void Song_PlayHandler(void){
	Note currentNote = *Song_CurrentNote(); // Finds current note using fuction
	beatIndex += 1;
	if(beatIndex >= currentNote.duration * 2) {
		noteIndex += 1;
		beatIndex = 0;
	}
}


void Rewind(void) {
	Pause();
	Song_PlayInit(currentSong); // Resets song
}
