#ifndef UTILITY
#define UTILITY

#include <sys/time.h>
#include <iostream>

// Timer struct
struct Timer
{
    struct timeval start_time;
    struct timeval end_time;
};

// Function to start timer
void startTimer(Timer* timer);

// Function to stop timer
void stopTimer(Timer* timer);

// Function to measure elapsed time
float elapsedTime(Timer timer);

#endif
