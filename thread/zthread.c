#include "../ztypes.h"
#include "zthread.h"

#ifdef _WIN32

#error Need windows thread implementation

#else

void* th_wrapper(void* data) {

	zthread_t* th = data;
	th->func(th);
	th->finished=ZTRUE;
	return NULL;
}


/* The first created thread will call ram_enter_mt, which creates the lock for ram debugging*/
static zbool zthread_ram_mt = ZFALSE;

/* Returns true for success */

zbool zthread_start(zthread_t* th,   void* func) 
{
	if (th == NULL)
		return ZFALSE;  /* Not valid*/

	th->func = func;
	th->finished = ZFALSE;  

	if (!pthread_create(&th->th_id, NULL, th_wrapper, th))
	{
		th->id_valid = ZTRUE;
		return ZTRUE;
	}

	/* did not start */

	th->id_valid = ZFALSE;

	return ZFALSE;
}

zbool zthread_isfinished(volatile zthread_t* th) 
{
	return th->finished;
}

zbool zthread_join(zthread_t* th) 
{
	if (!th->id_valid)
		return ZFALSE;

	if (!pthread_join(th->th_id, NULL))
		return ZTRUE;  //successfully joined

	return ZFALSE;  // did not join
}


/* Atomic increment, decrement operations */

zuint32 zlock_inc(zuint32* i) {
        return __sync_add_and_fetch(i,1);
}

zuint32 zlock_dec(zuint32* i) {
        return __sync_add_and_fetch(i,-1);
}



#endif

