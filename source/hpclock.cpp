#include "voxelquest/hpclock.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#define B3_USE_WINDOWS_TIMERS
#else
#include <sys/time.h>
#endif

struct HPClockData
{

#ifdef B3_USE_WINDOWS_TIMERS
    LARGE_INTEGER mClockFrequency;
    DWORD mStartTick;
    LONGLONG mPrevElapsedTime;
    LARGE_INTEGER mStartTime;
#else
#ifdef __CELLOS_LV2__
    uint64_t	mStartTime;
#else
    struct timeval mStartTime;
#endif
#endif //__CELLOS_LV2__

};

///The HPClock is a portable basic clock that measures accurate time in seconds, use for profiling.
HPClock::HPClock()
{
    m_data=new HPClockData;
#ifdef B3_USE_WINDOWS_TIMERS
    QueryPerformanceFrequency(&m_data->mClockFrequency);
#endif
    reset();
}

HPClock::~HPClock()
{
    delete m_data;
}

HPClock::HPClock(const HPClock& other)
{
    m_data=new HPClockData;
    *m_data=*other.m_data;
}

HPClock& HPClock::operator=(const HPClock& other)
{
    *m_data=*other.m_data;
    return *this;
}


/// Resets the initial reference time.
void HPClock::reset()
{
#ifdef B3_USE_WINDOWS_TIMERS
    QueryPerformanceCounter(&m_data->mStartTime);
    m_data->mStartTick=GetTickCount();
    m_data->mPrevElapsedTime=0;
#else
#ifdef __CELLOS_LV2__

    typedef uint64_t  ClockSize;
    ClockSize newTime;
    //__asm __volatile__( "mftb %0" : "=r" (newTime) : : "memory");
    SYS_TIMEBASE_GET(newTime);
    m_data->mStartTime=newTime;
#else
    gettimeofday(&m_data->mStartTime, 0);
#endif
#endif
}

/// Returns the time in ms since the last call to reset or since 
/// the HPClock was created.
unsigned long int HPClock::getTimeMilliseconds()
{
#ifdef B3_USE_WINDOWS_TIMERS
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    LONGLONG elapsedTime=currentTime.QuadPart-
        m_data->mStartTime.QuadPart;
    // Compute the number of millisecond ticks elapsed.
    unsigned long msecTicks=(unsigned long)(1000*elapsedTime/
        m_data->mClockFrequency.QuadPart);
    // Check for unexpected leaps in the Win32 performance counter.  
// (This is caused by unexpected data across the PCI to ISA 
    // bridge, aka south bridge.  See Microsoft KB274323.)
    unsigned long elapsedTicks=GetTickCount()-m_data->mStartTick;
    signed long msecOff=(signed long)(msecTicks-elapsedTicks);
    if(msecOff<-100||msecOff > 100)
    {
        // Adjust the starting time forwards.
        LONGLONG msecAdjustment=HPClockMin(msecOff *
            m_data->mClockFrequency.QuadPart/1000, elapsedTime-
            m_data->mPrevElapsedTime);
        m_data->mStartTime.QuadPart+=msecAdjustment;
        elapsedTime-=msecAdjustment;

        // Recompute the number of millisecond ticks elapsed.
        msecTicks=(unsigned long)(1000*elapsedTime/
            m_data->mClockFrequency.QuadPart);
    }

    // Store the current elapsed time for adjustments next time.
    m_data->mPrevElapsedTime=elapsedTime;

    return msecTicks;
#else

#ifdef __CELLOS_LV2__
    uint64_t freq=sys_time_get_timebase_frequency();
    double dFreq=((double)freq)/1000.0;
    typedef uint64_t  ClockSize;
    ClockSize newTime;
    SYS_TIMEBASE_GET(newTime);
    //__asm __volatile__( "mftb %0" : "=r" (newTime) : : "memory");

    return (unsigned long int)((double(newTime-m_data->mStartTime))/dFreq);
#else

    struct timeval currentTime;
    gettimeofday(&currentTime, 0);
    return (currentTime.tv_sec-m_data->mStartTime.tv_sec)*1000+
        (currentTime.tv_usec-m_data->mStartTime.tv_usec)/1000;
#endif //__CELLOS_LV2__
#endif
}

/// Returns the time in us since the last call to reset or since 
/// the Clock was created.
unsigned long int HPClock::getTimeMicroseconds()
{
#ifdef B3_USE_WINDOWS_TIMERS
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    LONGLONG elapsedTime=currentTime.QuadPart-
        m_data->mStartTime.QuadPart;

    // Compute the number of millisecond ticks elapsed.
    unsigned long msecTicks=(unsigned long)(1000*elapsedTime/
        m_data->mClockFrequency.QuadPart);

    // Check for unexpected leaps in the Win32 performance counter.  
    // (This is caused by unexpected data across the PCI to ISA 
    // bridge, aka south bridge.  See Microsoft KB274323.)
    unsigned long elapsedTicks=GetTickCount()-m_data->mStartTick;
    signed long msecOff=(signed long)(msecTicks-elapsedTicks);
    if(msecOff<-100||msecOff > 100)
    {
        // Adjust the starting time forwards.
        LONGLONG msecAdjustment=HPClockMin(msecOff *
            m_data->mClockFrequency.QuadPart/1000, elapsedTime-
            m_data->mPrevElapsedTime);
        m_data->mStartTime.QuadPart+=msecAdjustment;
        elapsedTime-=msecAdjustment;
    }

    // Store the current elapsed time for adjustments next time.
    m_data->mPrevElapsedTime=elapsedTime;

    // Convert to microseconds.
    unsigned long usecTicks=(unsigned long)(1000000*elapsedTime/
        m_data->mClockFrequency.QuadPart);

    return usecTicks;
#else

#ifdef __CELLOS_LV2__
    uint64_t freq=sys_time_get_timebase_frequency();
    double dFreq=((double)freq)/1000000.0;
    typedef uint64_t  ClockSize;
    ClockSize newTime;
    //__asm __volatile__( "mftb %0" : "=r" (newTime) : : "memory");
    SYS_TIMEBASE_GET(newTime);

    return (unsigned long int)((double(newTime-m_data->mStartTime))/dFreq);
#else

    struct timeval currentTime;
    gettimeofday(&currentTime, 0);
    return (currentTime.tv_sec-m_data->mStartTime.tv_sec)*1000000+
        (currentTime.tv_usec-m_data->mStartTime.tv_usec);
#endif//__CELLOS_LV2__
#endif 
}