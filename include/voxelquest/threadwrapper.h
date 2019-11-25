#ifndef _voxelquest_threadwrapper_h_
#define _voxelquest_threadwrapper_h_


class ThreadWrapper {
private:
	bool threadRunningEx; // thread is running (exclusive, must lock)
public:
	std::thread threadMain;
	std::mutex threadMutex;
	bool threadRunning; // thread is running (not exclusive)
	
	int threadDataInt[THREAD_DATA_COUNT];
	
    ThreadWrapper();
	
    void init();
	
    void setRunningLocked(bool val) \;
	
    bool isReady();
	
};

#endif//_voxelquest__h_