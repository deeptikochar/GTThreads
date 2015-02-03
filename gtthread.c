#include "gtthread.h"

_Atomic(int) *num_threads = 0;

Qnode *scheduler_head = NULL;
Qnode *scheduler_tail = NULL;

void gtthread_init(long period)
{
    ucontext_t current;
    getcontext(&current);

    Qnode *initial_node = malloc(sizeof(Qnode*));
    initial_node->context = current;
    initial_node->next = NULL;
    initial_node->thread_id = generate_thread_id();

    gtthread *initial_thread = malloc(sizeof(gtthread*));
    initial_thread->thread_id = initial_node->thread_id;
    initial_thread->status = ACTIVE;
    initial_thread->retval = NULL;

    enqueue_thread_list(initial_thread);
    enqueue_sched(initial_node);

    
    // Setting the timer
    struct sigaction sig_act;
    struct itimerval timer;

    memset(&sig_act, 0, sizeof(&sig_act));
    sig_act.sa_handler = &scheduler;
    sigaction(SIGVTALARM, &sig_act, NULL);

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 250000;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 250000;
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

int  gtthread_create(gtthread_t *thread,
                     void *(*start_routine)(void *),
                     void *arg)
{
    int retval = 0;
    //The value can change after reading - need to fix this. Disable context switch
    if(*num_threads < MAX_THREADS)
    {
        atomic_fetch_add(num_threads, 1);
    }
    else
        return -1;

    gtthread *new_gtthread = malloc(sizeof(gtthread*));
    
    *thread = generate_thread_id();                   
    new_gtthread->thread_id = *thread;
    new_gtthread->status = ACTIVE;                    // Need to decide what status to give it
    new_gtthread->retval = NULL;
    enqueue_thread_list(new_gtthread);                            // Implement enqueue, dequeue, head and tail
    
    ucontext_t new_thread;
    getcontext(&new_thread);
    
    new_thread.uc_link = 0;
    new_thread.uc_stack_ss_sp = malloc(STACK_SIZE);
    new_thread.uc_stack_ss_size = STACK_SIZE;
    new_thread.uc_stack_ss_flags = 0;
    makecontext(&new_thread, gtthread_run, 1, start_routine, arg);  

    Qnode *new_node = malloc(sizeof(Qnode*));
    new_node->context = new_thread;
    new_node->thread_id = *thread;
    new_node->next = NULL;

    retval = enqueue_sched(new_thread);
    if(retval < 0)
        return -1;
    //how to call pthread_exit for this thread
}


int  gtthread_join(gtthread_t thread, void **status);

void gtthread_exit(void *retval)
{
    atomic_fetch_sub(num_threads, 1);
    //free the Qnode
    //set status of gtthread to FINISHED
    //set retval of gtthread
}


int  gtthread_yield(void);
int  gtthread_equal(gtthread_t t1, gtthread_t t2);
int  gtthread_cancel(gtthread_t thread);
gtthread_t gtthread_self(void);


int  gtthread_mutex_init(gtthread_mutex_t *mutex);
int  gtthread_mutex_lock(gtthread_mutex_t *mutex);
int  gtthread_mutex_unlock(gtthread_mutex_t *mutex);

int gtthread_run(void* (*start_routine)(void*), void *arg)
{
    void *retval;
    //need to set retval
    start_routine(arg);
    gtthread_exit(retval);
}

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

void scheduler(int signum)
{

}

int enqueue_sched(Qnode *new_node)
{
    if(scheduler_tail == NULL)
    {
        if(scheduler_head == NULL)
        {
            scheduler_head = new_node;
            scheduler_tail = new_node;
            return 0;
        }    
        return -1;
    }
    scheduler_tail->next = new_node;
    new_node->next = NULL;
    return 0;
}

Qnode* dequeue_sched()
{
    Qnode *ptr = scheduler_head;
    if(ptr == NULL)
        return NULL;
    scheduler_head = scheduler_head->next;
    return ptr;
}