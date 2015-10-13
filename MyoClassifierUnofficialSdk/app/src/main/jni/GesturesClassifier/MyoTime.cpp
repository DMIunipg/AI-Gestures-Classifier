#include "MyoTime.h"

#if defined(__APPLE__) || defined(__linux) || defined(__unix)
    #include <sys/time.h>
#elif defined(_WIN32)
    #include <windows.h>
#else
    #error "Un support OS"
#endif

//cpu Ticks
uint64_t myo::GetTimeTicks()
{
    uint64_t val;
#if defined(__APPLE__) || defined(__linux) || defined(__unix)
    timeval timeVal;

    gettimeofday( &timeVal, NULL );

    val = (uint64_t)timeVal.tv_sec * (1000*1000) + (uint64_t)timeVal.tv_usec;
#else
    QueryPerformanceCounter( (LARGE_INTEGER *)&val );
#endif
    return val;
}
//time
double myo::GetTime()
{
    static double	coe;
#if defined(__APPLE__) || defined(__linux) || defined(__unix)
    coe = 1.0 / 1000.0;
#else
    static uint64_t freq;

    if ( freq == 0 )
    {
        QueryPerformanceFrequency( (LARGE_INTEGER *)&freq );
        coe = 1000.0 / freq;
    }
#endif

    return (double)GetTimeTicks() * coe;
}
