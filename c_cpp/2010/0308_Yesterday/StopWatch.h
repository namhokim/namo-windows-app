#ifndef	_STOP_WATCH_H_
#define	_STOP_WATCH_H_
#include <Windows.h>

class StopWatch
{
public:
	StopWatch() {
		QueryPerformanceFrequency(&ticksPerSecond);
	}

	void Start() {
		QueryPerformanceCounter(&start_ticks);
	}

	void Stop() {
		QueryPerformanceCounter(&end_ticks);
		diff_ticks.QuadPart = end_ticks.QuadPart- start_ticks.QuadPart;
	}

	double GetDiff() {
		return ( (double)diff_ticks.QuadPart/(double)ticksPerSecond.QuadPart);
	}

private:
	LARGE_INTEGER ticksPerSecond;
	LARGE_INTEGER start_ticks, end_ticks, diff_ticks;  
};

#endif