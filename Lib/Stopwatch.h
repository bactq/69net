#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

//#include <chrono>
//#include <stdint.h>
//typedef int8_t  int8, sbyte;
//typedef int16_t int16;
//typedef int32_t int32;
//typedef int64_t int64;
//
//typedef uint8_t  uint8, byte;
//typedef uint16_t uint16, ushort;
//typedef uint32_t uint32, uint;
//typedef uint64_t uint64;

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::time_point;
using std::chrono::hours;
using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

class Stopwatch
{
public:
    Stopwatch() : _begin( high_resolution_clock::now() ) {}
    void reset() { _begin = high_resolution_clock::now(); }

    //Ĭ���������
    inline int64_t elapsedMillseconds() const
    {
        return duration_cast<milliseconds>( high_resolution_clock::now() - _begin ).count();
    }

    //΢��
    inline int64_t elapsedMicroseconds() const
    {
        return duration_cast<microseconds>( high_resolution_clock::now() - _begin ).count();
    }

    //����
    inline int64_t elapsedNanoseconds() const
    {
        return duration_cast<nanoseconds>( high_resolution_clock::now() - _begin ).count();
    }

    //��
    inline int64_t elapsedSeconds() const
    {
        return duration_cast<seconds>( high_resolution_clock::now() - _begin ).count();
    }

    //��
    inline int64_t elapsedMinutes() const
    {
        return duration_cast<minutes>( high_resolution_clock::now() - _begin ).count();
    }

    //ʱ
    inline int64_t elapsedHours() const
    {
        return duration_cast<hours>( high_resolution_clock::now() - _begin ).count();
    }

private:
    time_point<high_resolution_clock> _begin;
};

#endif
