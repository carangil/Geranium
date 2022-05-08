// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2018 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#define ZTHREAD_SIMPLE_ATOMICS



#ifdef _WIN32

#include <windows.h>

typedef struct zthread_s {

	HANDLE thh;
	zbool id_valid;
	zbool finished;
	void(*func) (struct zthread_s* th);
} zthreadT;

typedef HANDLE  zlockT;

#define zlock_init(ZL)           *(ZL)=CreateMutex(NULL,FALSE,NULL)
#define zlock_destroy(ZL)        CloseHandle(*ZL)

void zlock(zlockT* lk);
zbool ztrylock(zlockT* lk);
void zunlock(zlockT* lk);



#else

#include <pthread.h>

typedef struct zthread_s {
	pthread_t th_id;
	zint32 is_finished;
	zbool id_valid;
        void (*func) (struct zthread_s* th);
	void* data;
} zthreadT;


typedef pthread_mutex_t zlockT;

#define zlock_init(ZL)           pthread_mutex_init( (ZL), NULL)
#define zlock_destroy(ZL)        pthread_mutex_destroy( (ZL))

#define zlock(ZL)        (void)pthread_mutex_lock(ZL)
#define ztrylock(ZL)     (pthread_mutex_trylock(ZL)==0)
#define zunlock(ZL)      (void)pthread_mutex_unlock(ZL)


#endif


/* Atomic increment, decrement */
zint32 zlock_inc(zint32* i);
zint32 zlock_dec(zint32* i);
zint32 zlock_get(zint32* i);
        
/* Threads*/       


zbool zthread_start(zthreadT* th, void (*func) (struct zthread_s* th));
zbool zthread_isfinished(zthreadT* th) ;  
zbool zthread_join(zthreadT* th);

