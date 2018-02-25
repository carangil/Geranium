// projectZ - This file is part of a project named 'projectZ'
// ProjectZ is (C) 2010 Mark W. Sherman, all rights reserved.
// Commercial use prohibited.

#define ZTHREAD_SIMPLE_ATOMICS



#ifdef _WIN32

#include <windows.h>

typedef struct zthread_s {

	HANDLE thh;
	zbool id_valid;
	zbool finished;
	void(*func) (struct zthread_s* th);
} zthread_t;

typedef HANDLE  zlock_t;

#define zlock_init(ZL)           *(ZL)=CreateMutex(NULL,FALSE,NULL)
#define zlock_destroy(ZL)        CloseHandle(*ZL)

void zlock(zlock_t* lk);
zbool ztrylock(zlock_t* lk);
void zunlock(zlock_t* lk);


//#error TODO WINDOWS THREAD IMPLEMENTATION

#else

#include <pthread.h>

typedef struct zthread_s {
	pthread_t th_id;
	zbool id_valid;
	zbool finished;
        void (*func) (struct zthread_s* th);
} zthread_t;


typedef pthread_mutex_t zlock_t;

#define zlock_init(ZL)           pthread_mutex_init( (ZL), NULL)
#define zlock_destroy(ZL)        pthread_mutex_destroy( (ZL))

#define zlock(ZL)        (void)pthread_mutex_lock(ZL)
#define ztrylock(ZL)     (pthread_mutex_trylock(ZL)==0)
#define zunlock(ZL)      (void)pthread_mutex_unlock(ZL)


#endif


/* Atomic increment, decrement */
zuint32 zlock_inc(zuint32* i);
zuint32 zlock_dec(zuint32* i);
        
/* Threads*/       


zbool zthread_start(zthread_t* th, void (*func) (struct zthread_s* th) );
zbool zthread_isfinished(volatile zthread_t* th) ;  
zbool zthread_join(zthread_t* th);

