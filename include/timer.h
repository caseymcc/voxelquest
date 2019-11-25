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
#ifdef WIN32
    LARGE_INTEGER frequency;                    // ticks per second
    LARGE_INTEGER startCount;                   //
    LARGE_INTEGER endCount;                     //
#else
    timeval startCount;                         //
    timeval endCount;                           //
#endif
};