#ifndef __GTTHREAD_H
#define __GTTHREAD_H



#define MAX_NUM_THREADS 150
#define STACK_SIZE 10240


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include "structures.h"


typedef struct gtthread_mutex_t
{
	int lock;                         /* 0 -locked, 1-unlocked, 2 - not initialized */
	gtthread_t owner;
} gtthread_mutex_t;

void gtthread_init(long period);
int  gtthread_create(gtthread_t *thread,
                     void *(*start_routine)(void *),
                     void *arg);
int  gtthread_join(gtthread_t thread, void **status);
void gtthread_exit(void *retval);
int gtthread_yield(void);
int  gtthread_equal(gtthread_t t1, gtthread_t t2);
int  gtthread_cancel(gtthread_t thread);
gtthread_t gtthread_self(void);

void gtthread_run(void* (*start_routine)(void*), void *arg);
gtthread_t generate_thread_id();

void block_signal();
void unblock_signal();
void gtthread_scheduler(int signum);


int  gtthread_mutex_init(gtthread_mutex_t *mutex);
int  gtthread_mutex_lock(gtthread_mutex_t *mutex);
int  gtthread_mutex_unlock(gtthread_mutex_t *mutex);


#endif /* __GTTHREAD_H */
