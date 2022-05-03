#include "../ztypes.h"
#include "ztime.h"




#ifdef _WIN32

zbool g_tm_init = ZFALSE;
LARGE_INTEGER g_tm_perf_frequency;

zuint32 tm_diff_microstamp_us(tm_microstamp_t* before, tm_microstamp_t* after)
{
	LARGE_INTEGER diff;
	zuint32		  diff32 =  0;

	if (!g_tm_init)
	{
		QueryPerformanceFrequency(&g_tm_perf_frequency);
		g_tm_init = ZTRUE;
	}

	if ((g_tm_perf_frequency.QuadPart == 0) || (after == NULL) || (before == NULL))
	{
		return 0; /* We are in trouble */
	}
	
	diff.QuadPart = after->_ticks.QuadPart - before->_ticks.QuadPart ;
	diff.QuadPart *= 1000000;
	diff32 = (diff.QuadPart) / (g_tm_perf_frequency.QuadPart);

	return diff32;
}

#else

zuint32 tm_diff_microstamp_us(tm_microstamp_t* before, tm_microstamp_t* after)
{
	int udiff;
	int sdiff = ((int)after->tv_sec) - ((int)before->tv_sec);
	udiff = ((int)after->tv_usec) - ((int)before->tv_usec);

	//printf("  %d:%d  - %d:%d \n", (int)after->tv_sec, (int)after->tv_usec, (int)before->tv_sec, (int)before->tv_usec);


	return (sdiff*1000000) + udiff;
}
#endif
