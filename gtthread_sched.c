#include "gtthread.h"

void block_signal()
{
    sigprocmask(SIG_BLOCK, &mask, NULL);
}

void unblock_signal()
{
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
}


void gtthread_scheduler(int signum)
{
    struct Qnode *next_Qnode;
    struct Qnode *prev_Qnode;
    block_signal();

    next_Qnode = dequeue_sched();
    if(next_Qnode == NULL) 
    {                             /*There are no other threads */
        unblock_signal();
        return;
    }

    if(signum >= 0)
    {
        enqueue_sched(current_Qnode);                   /* enqueue current_Qnode */
        prev_Qnode = current_Qnode;
    }
    /* Set current_Qnode to next_Qnode */
 
    current_Qnode = next_Qnode;
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
    /* switch context */
    if(signum >= 0)
    {
        if(swapcontext(&(prev_Qnode->context), &(current_Qnode->context)) < 0)          
            return;

    }
    else
    {
        setcontext(&current_Qnode->context);
    }
    unblock_signal();
    
}
