#include "voxelquest/threadwrapper.h"


ThreadWrapper::ThreadWrapper()
{

}

void ThreadWrapper::init()
{
    threadRunning=false;
    threadRunningEx=false;
}

void ThreadWrapper::setRunningLocked(bool val)
{
    threadMutex.lock();
    threadRunningEx=val;
    threadMutex.unlock();
}

bool ThreadWrapper::isReady()
{
    bool doProc=false;

    if(threadRunning)
    {
        if(threadMutex.try_lock())
        {
            if(threadRunningEx)
            {

            }
            else
            {
                doProc=true;
            }
            threadMutex.unlock();
        }
    }
    else
    {
        doProc=true;
    }

    return doProc;
}
