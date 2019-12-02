#ifndef _voxelquest_threadpoolwrapper_h_
#define _voxelquest_threadpoolwrapper_h_

#include "voxelquest/threadwrapper.h"

#include <vector>

enum E_THREAD_TYPE
{
	E_TT_GENPATHS,
	E_TT_GENLIST,
	E_TT_SPEECH,
	E_TT_LENGTH
};

class Singleton;

class ThreadPoolWrapper
{
public:
    ThreadPoolWrapper();
    ~ThreadPoolWrapper();

    void init(Singleton* _singleton, int _maxThreads, bool _singleThreaded);

    void doSpeak(std::string speechString);

    void funcTP(int threadId);
    void startTP(int threadId);
    bool stopTP(int threadId);

    // must set intData first
    // must set stringData as well

    bool anyThreadAvail();

    bool startThread(bool checkAvail=true);

    bool anyRunning();

    void stopAll();

//private:
    int maxThreads;
    int intData[THREAD_DATA_COUNT];
    std::string stringData[THREAD_DATA_COUNT];

    ThreadWrapper* threadPool;
    Singleton* singleton;

    bool singleThreaded;

    std::vector<int> availIds;
};

#endif//_voxelquest__h_