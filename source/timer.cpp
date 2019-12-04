#include "timer.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

struct TimerHidden
{
#ifdef WIN32
    LARGE_INTEGER frequency;                    // ticks per second
    LARGE_INTEGER startCount;                   //
    LARGE_INTEGER endCount;                     //
#else
    timeval startCount;                         //
    timeval endCount;                           //
#endif
};

Timer::Timer()
{
    m_hidden.reset(new TimerHidden());
#ifdef WIN32
    QueryPerformanceFrequency(&m_hidden->frequency);
    m_hidden->startCount.QuadPart=0;
    m_hidden->endCount.QuadPart=0;
#else
    m_hidden->startCount.tv_sec=m_hidden->startCount.tv_usec=0;
    m_hidden->endCount.tv_sec=m_hidden->endCount.tv_usec=0;
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
    QueryPerformanceCounter(&m_hidden->startCount);
#else
    gettimeofday(&m_hidden->startCount, NULL);
#endif
}
void   Timer::stop()
{
    stopped=1; // set timer stopped flag

#ifdef WIN32
    QueryPerformanceCounter(&m_hidden->endCount);
#else
    gettimeofday(&m_hidden->endCount, NULL);
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
        QueryPerformanceCounter(&m_hidden->endCount);
    }

    startTimeInMicroSec=m_hidden->startCount.QuadPart * (1000000.0/m_hidden->frequency.QuadPart);
    endTimeInMicroSec=m_hidden->endCount.QuadPart * (1000000.0/m_hidden->frequency.QuadPart);
#else
    if(!stopped)
    {
        gettimeofday(&m_hidden->endCount, NULL);
    }

    startTimeInMicroSec=(m_hidden->startCount.tv_sec * 1000000.0)+m_hidden->startCount.tv_usec;
    endTimeInMicroSec=(m_hidden->endCount.tv_sec * 1000000.0)+m_hidden->endCount.tv_usec;
#endif

    return endTimeInMicroSec-startTimeInMicroSec;
}

