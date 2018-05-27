// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.


#ifdef _WIN32

#include <windows.h>


typedef struct tm_microstamp_s
{
	LARGE_INTEGER _ticks;
} tm_microstamp_t;

extern tm_microstamp_t tm_zero_microstamp;

#define tm_get_microstamp(zzzstamp)  {(*zzzstamp)._ticks.QuadPart = 0; QueryPerformanceCounter( & ((*(zzzstamp)) ._ticks)) ; }

#else

typedef struct timeval tm_microstamp_t;

#define tm_get_microstamp(zzzstamp)   gettimeofday(zzzstamp, NULL)


#define tm_msleep(x)  usleep((x)*1000)

#endif

// Records current time with precision approaching microseconds
//void tm_get_microstamp(tm_microstamp_t* stamp);

// Returns difference in two time stamps.  In microseconds.  2^32 is max number of microseconds possible 
// (about 70 minutes)
//  Microstamps are only valid if taken in the same invokation of a program. (Can't be saved in disc, etc)
zuint32 tm_diff_microstamp_us(tm_microstamp_t* before, tm_microstamp_t* after);



