#pragma once

#define TIMER_STR_BUFFER_BYTES 32
extern char timer_str_buffer[TIMER_STR_BUFFER_BYTES];
#define TIME_STR_BUFFER_BYTES 32
extern char time_str_buffer[TIME_STR_BUFFER_BYTES];

extern int timer_counter;
extern int hours;
extern int minutes;
extern int seconds;

extern bool started;

void parse_time(void);