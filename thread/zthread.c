#include "ztypes.h"
#include "zthread.h"
#include <stdio.h>

#ifdef _WIN32

//#error Need windows thread implementation



zint32 zlock_inc(zint32* i) {
	//return ++(*i);//fake
	
	return InterlockedIncrement(i);
}

zint32 zlock_dec(zint32* i) {

	//return --(*i);//fake
	
	return InterlockedDecrement(i);
}

void zlock(zlockT* lk) {
	int r = WaitForSingleObject(*lk, INFINITE);
	if (r != WAIT_OBJECT_0)
		printf(" CRAP WaitForSingleObject returned %x\n", r);
}

void zunlock(zlockT* lk) {
	ReleaseMutex(*lk);
}

zbool ztrylock(zlockT*lk) {

	int r = WaitForSingleObject(*lk, 0);
	if (r == WAIT_OBJECT_0)
		return ZTRUE;
	
	return ZFALSE;

}


DWORD WINAPI th_wrapper(LPVOID pv) {
	zthreadT* th = pv;
	th->func(th);
	th->finished=ZTRUE;
	return 0;
}



zbool zthread_start(zthreadT* th, void(*func) (struct zthread_s* th) ) {
	th->finished = ZFALSE;
	th->func = func;
	th->thh = CreateThread(NULL, 0, th_wrapper, (void*)th, 0, NULL);
	printf("attempt to make thread %p\n",  th->thh);
	if (th->thh != NULL)
		return ZTRUE;
	else
		return ZFALSE;
}

zbool zthread_isfinished(volatile zthreadT* th) {
	return th->finished;
}

zbool zthread_join(zthreadT* th) {
	WaitForSingleObject(th->thh, INFINITE);
	CloseHandle(th->thh);
	return TRUE;
}

#else

void* th_wrapper(void* data) {

	zthreadT* th = data;
	th->func(th);
	zlock_inc(&th->is_finished); //should be setting 0 to 1
	return NULL;
}


/* The first created thread will call ram_enter_mt, which creates the lock for ram debugging*/
static zbool zthread_ram_mt = ZFALSE;

/* Returns true for success */

zbool zthread_start(zthreadT* th, void (*func) (struct zthread_s* th)) 
{

	if (th == NULL)
		return ZFALSE;  /* Not valid*/

	th->func = func;
	th->is_finished = 0;

	if (!pthread_create(&th->th_id, NULL, th_wrapper, th))
	{
		th->id_valid = ZTRUE;
		return ZTRUE;
	}

	/* did not start */

	th->id_valid = ZFALSE;

	return ZFALSE;
}

zbool zthread_isfinished(zthreadT* th) 
{
	return zlock_get(&th->is_finished);
}

zbool zthread_join(zthreadT* th) 
{
	if (!th->id_valid)
		return ZFALSE;

	if (!pthread_join(th->th_id, NULL))
		return ZTRUE;  //successfully joined

	return ZFALSE;  // did not join
}


/* Atomic increment, decrement operations */

zint32 zlock_inc(zuint32* i) {
        return __sync_add_and_fetch(i,1);
}

zint32 zlock_dec(zuint32* i) {
        return __sync_add_and_fetch(i,-1);
}

zint32 zlock_get(zuint32* i) {
        return __sync_add_and_fetch(i,0);
}


#endif

