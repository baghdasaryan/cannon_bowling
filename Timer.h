#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef _WIN32
#include <windows.h>

class Timer
{
public:
	Timer();
	float GetElapsedTime();
	void Reset();

private:
	LONGLONG cur_time;

	DWORD time_count;
	LONGLONG perf_cnt;
	bool perf_flag;
	LONGLONG last_time;
	float time_scale;

	bool QPC;
};

inline void Timer::Reset()
{
	last_time = cur_time;
}


#else	// Unix based OS
#include <sys/time.h>

class Timer
{
public:
	Timer();

	void Reset();
	float GetElapsedTime();

private:
	timeval cur_time;

};

inline void Timer::Reset()
{
	gettimeofday(&cur_time, 0);
}

#endif  //_WIN32

#endif  // __TIMER_H__
