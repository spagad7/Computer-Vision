#include "../include/utility.hpp"


// Function to start timer
void startTimer(Timer* timer)
{
    gettimeofday(&(timer->start_time), NULL);
}


// Function to stop timer
void stopTimer(Timer* timer)
{
    gettimeofday(&(timer->end_time), NULL);
}


// Function to measure elapsed time
float elapsedTime(Timer timer)
{
    float elptime = (float)(timer.end_time.tv_sec - timer.start_time.tv_sec);
    elptime += (float)((timer.end_time.tv_usec - timer.start_time.tv_usec)
                /1.0e6);
    return elptime;
}
