#include "voice.h"
#include <stdio.h>
#include <string.h>


int getMode(char* buffer) {
	for (int i = 0; i < 4; i++) {
		if (strcmp(buffer, commands[i])) {
			return i;
		}
	}
	
	if (strncmp(buffer, "set an alarm for ", 17)) {
		return parseTime(buffer+17);
	}
	return -1;
}

int parseTime(char* time) {
	char character = *time;
	
	return 0;
}
