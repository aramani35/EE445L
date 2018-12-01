// voice.h
// Method for getting mode from voice through BT/Grove


const char* commands[] = {
	"Go home",
	"What's the weather", 
	"Play music",
	"What time is it",
	
};

int getMode(char buffer[20]);

int parseTime(char* time);
