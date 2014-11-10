#include <pebble.h>
#include "timer.h"

#define TIMER_STR_BUFFER_BYTES 32
char timer_str_buffer[TIMER_STR_BUFFER_BYTES];
#define TIME_STR_BUFFER_BYTES 32
char time_str_buffer[TIME_STR_BUFFER_BYTES];

int timer_counter = 0;
int hours, minutes, seconds;

bool started = false;

void parse_time(void) {
	hours = (timer_counter / 60 / 60) % 24;
	minutes = (timer_counter / 60) % 60;
	seconds = timer_counter % 60;
}