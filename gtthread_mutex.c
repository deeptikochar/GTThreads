#include "gtthread.h"


int  gtthread_mutex_init(gtthread_mutex_t *mutex)
{
	if(mutex == NULL)
		return -1;
    mutex->lock = 0;
    mutex->owner = 0;
    return 0;   
}

int  gtthread_mutex_lock(gtthread_mutex_t *mutex)
{
    block_signal();
    if(mutex->lock == 2)
    {
        unblock_signal();
        return -1;
    }
    if(mutex->lock == 1 && mutex->owner == gtthread_self())
    {
        unblock_signal();
        return 0;
    }
    
    while(1)
    {
        if(mutex->lock == 1)
            gtthread_yield();
        else if(mutex->lock == 0)
        {
            mutex->lock = 1;   
            mutex->owner = gtthread_self();       
            break;
        }
    }
    unblock_signal();
    return 0;
}
int  gtthread_mutex_unlock(gtthread_mutex_t *mutex)
{
    block_signal();
    if(mutex->lock == 2)
    {
        unblock_signal();
        return -1;
    }
    if(mutex->owner != gtthread_self())
    {   
        unblock_signal();
        return -1;
    }
    mutex->lock = 0;
    unblock_signal();
    return 0;
}