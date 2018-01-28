#include "hiresclock.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

/*=========================================*/
double swHiResClock(void) {
#ifdef _WIN32
	LARGE_INTEGER frequency;
    LARGE_INTEGER time;
	
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&time);
	
	return (double)time.QuadPart / (double)frequency.QuadPart;
#else
	timeval time;
	
	gettimeofday(&time, NULL);
	
	return (double)time.tv_sec;
#endif
}
