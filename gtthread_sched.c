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
    block_signal();

    
    struct Qnode *next_Qnode;
    struct Qnode *prev_Qnode;
    next_Qnode = dequeue_sched();
//    printf("here");
    if(next_Qnode == NULL) 
    {    
//        printf("Next node is null\n");                         //There are no other threads 
        unblock_signal();
        return;
    }
 //   printf("Dequeued node id %lu\n", next_Qnode->thread_id);
    if(signum >= 0)
    {
        enqueue_sched(current_Qnode);                   // enqueue current_Qnode
        prev_Qnode = current_Qnode;
    }
    // Set current_Qnode to next_Qnode
//    print_scheduler_Q();    
    current_Qnode = next_Qnode;
//    printf("Current thread is %lu\n", gtthread_self() );
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
    // switch context
    if(signum >= 0)
    {
        if(swapcontext(&(prev_Qnode->context), &(current_Qnode->context)) < 0)                //if there is an error - decide what to do here
            exit;

    }
    else
    {
        setcontext(&current_Qnode->context);
    }
    unblock_signal();
    
}