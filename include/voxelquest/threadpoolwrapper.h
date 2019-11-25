#ifndef _voxelquest_threadpoolwrapper_h_
#define _voxelquest_threadpoolwrapper_h_

class ThreadPoolWrapper
{
public:
    ThreadPoolWrapper();
    ~ThreadPoolWrapper();

    void init(Singleton* _singleton, int _maxThreads, bool _singleThreaded);

    void doSpeak(string speechString);

    void funcTP(int threadId);
    void startTP(int threadId);
    bool stopTP(int threadId);

    // must set intData first
    // must set stringData as well

    bool anyThreadAvail();

    bool startThread(bool checkAvail=true);

    bool anyRunning();

    void stopAll();

private:
    int maxThreads;
    int intData[THREAD_DATA_COUNT];
    string stringData[THREAD_DATA_COUNT];

    ThreadWrapper* threadPool;
    Singleton* singleton;

    bool singleThreaded;

    std::vector<int> availIds;
};

#endif//_voxelquest__h_