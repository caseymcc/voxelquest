#ifndef _voxelquest_threadwrapper_h_
#define _voxelquest_threadwrapper_h_

#include <thread>
#include <mutex>

const static int THREAD_DATA_COUNT=16;

class ThreadWrapper {
public:
    ThreadWrapper();
	
    void init();
	
    void setRunningLocked(bool val);
	
    bool isReady();

	std::thread threadMain;
	std::mutex threadMutex;
	bool threadRunning; // thread is running (not exclusive)

	int threadDataInt[THREAD_DATA_COUNT];

private:
	bool threadRunningEx; // thread is running (exclusive, must lock)

};

#endif//_voxelquest__h_