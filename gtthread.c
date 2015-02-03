#include "gtthread.h"

_Atomic(int) *num_threads = 0;

void gtthread_init(long period)
{
    ucontext_t current;
    getcontext(&current);
}

int  gtthread_create(gtthread_t *thread,
                     void *(*start_routine)(void *),
                     void *arg)
{
    //The value can change after reading - need to fix this. Disable context switch
    if(*num_threads < MAX_THREADS)
    {
        atomic_fetch_add(num_threads, 1);
    }
    else
        return -1;

    gtthread *new_gtthread;
    
    *thread = generate_thread_id();                   
    new_gtthread->thread_id = *thread;
    new_gtthread->status = QUEUED;                    // Need to decide what status to give it
    enqueue(new_gtthread);                            // Implement enqueue, dequeue, head and tail
    
    ucontext_t new_thread;
    getcontext(new_thread);
    
    new_thread.uc_link = 0;
    new_thread.uc_stack_ss_sp = malloc(STACK_SIZE);
    new_thread.uc_stack_ss_size = STACK_SIZE;
    new_thread.uc_stack_ss_flags = 0;
    makecontext(&new_thread, start_routine, 1, arg);  

    //send to scheduler? 
    //how to call pthread_exit for this thread
}


int  gtthread_join(gtthread_t thread, void **status);

void gtthread_exit(void *retval)
{
    atomic_fetch_sub(num_threads, 1);
}


int  gtthread_yield(void);
int  gtthread_equal(gtthread_t t1, gtthread_t t2);
int  gtthread_cancel(gtthread_t thread);
gtthread_t gtthread_self(void);


int  gtthread_mutex_init(gtthread_mutex_t *mutex);
int  gtthread_mutex_lock(gtthread_mutex_t *mutex);
int  gtthread_mutex_unlock(gtthread_mutex_t *mutex);

gtthread_t generate_thread_id()
{
    gtthread_t thread_id;
    gtthread *ptr;
    bool exists;

    srand(time(NULL)); 
    while(1)
    {
        ptr = head;
        exists = false;
        thread_id = (unsigned long) rand();           // try making a long random number
        while(ptr != NULL)
        {
            if(ptr->thread_id == thread_id)
            {   
                exists = true;
                break;
            }
            ptr->ptr->next;
        }
        if(found == false)
             return thread_id;
     } 
}
