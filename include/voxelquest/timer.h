#ifndef _voxelquest_timer_h_
#define _voxelquest_timer_h_

#include <memory>

struct TimerHidden;

class Timer
{
public:
    Timer();
    ~Timer();

    void   start();
    void   stop();
    double getElapsedTime();
    double getElapsedTimeInSec();

    double getElapsedTimeInMilliSec();

    double getElapsedTimeInMicroSec();

protected:


private:
    double startTimeInMicroSec;                 // starting time in micro-second
    double endTimeInMicroSec;                   // ending time in micro-second
    int    stopped;                             // stop flag 
    std::unique_ptr<TimerHidden> m_hidden;
};

#endif//_voxelquest__h_