#include "timer.h"

Timer::Timer()
{
#ifdef WIN32
    QueryPerformanceFrequency(&frequency);
    startCount.QuadPart=0;
    endCount.QuadPart=0;
#else
    startCount.tv_sec=startCount.tv_usec=0;
    endCount.tv_sec=endCount.tv_usec=0;
#endif

    stopped=0;
    startTimeInMicroSec=0;
    endTimeInMicroSec=0;
}
Timer::~Timer()
{

}

void   Timer::start()
{
    stopped=0; // reset stop flag
#ifdef WIN32
    QueryPerformanceCounter(&startCount);
#else
    gettimeofday(&startCount, NULL);
#endif
}
void   Timer::stop()
{
    stopped=1; // set timer stopped flag

#ifdef WIN32
    QueryPerformanceCounter(&endCount);
#else
    gettimeofday(&endCount, NULL);
#endif
}
double Timer::getElapsedTime()
{
    return this->getElapsedTimeInSec();
}
double Timer::getElapsedTimeInSec()
{
    return this->getElapsedTimeInMicroSec() * 0.000001;
}

double Timer::getElapsedTimeInMilliSec()
{
    return this->getElapsedTimeInMicroSec() * 0.001;
}

double Timer::getElapsedTimeInMicroSec()
{
#ifdef WIN32
    if(!stopped)
    {
        QueryPerformanceCounter(&endCount);
    }

    startTimeInMicroSec=startCount.QuadPart * (1000000.0/frequency.QuadPart);
    endTimeInMicroSec=endCount.QuadPart * (1000000.0/frequency.QuadPart);
#else
    if(!stopped)
    {
        gettimeofday(&endCount, NULL);
    }

    startTimeInMicroSec=(startCount.tv_sec * 1000000.0)+startCount.tv_usec;
    endTimeInMicroSec=(endCount.tv_sec * 1000000.0)+endCount.tv_usec;
#endif

    return endTimeInMicroSec-startTimeInMicroSec;
}

