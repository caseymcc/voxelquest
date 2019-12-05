#ifndef _voxelquest_hpclock_h_
#define _voxelquest_hpclock_h_

class HPClock
{
public:
    HPClock();

    HPClock(const HPClock& other);
    HPClock& operator=(const HPClock& other);

    ~HPClock();

    /// Resets the initial reference time.
    void reset();

    /// Returns the time in ms since the last call to reset or since 
    /// the HPClock was created.
    unsigned long int getTimeMilliseconds();

    /// Returns the time in us since the last call to reset or since 
    /// the Clock was created.
    unsigned long int getTimeMicroseconds();
private:
    struct HPClockData* m_data;
};



template <class T>
const T& HPClockMin(const T& a, const T& b)
{
    return a<b?a:b;
}

#endif//_voxelquest_hpclock_h_