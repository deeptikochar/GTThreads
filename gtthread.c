#include "gtthread.h"

//_Atomic(int) *num_threads = 0;
int *num_threads = 0;
Qnode *current_Qnode = NULL;
struct sigaction sig_act;
struct itimerval timer;
Qnode *scheduler_head = NULL;
Qnode *scheduler_tail = NULL;
gtthread *gtthread_head = NULL;
gtthread *gtthread_tail = NULL;


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

    insert_thread_list(gtthread_head, gtthread_tail, initial_thread);
    enqueue_sched(scheduler_head, scheduler_tail, initial_node);
    current_Qnode = initial_node;

    
    // Setting the timer

    memset(&sig_act, 0, sizeof(&sig_act));
    sig_act.sa_handler = &gtthread_scheduler;
    sigaction(SIGVTALRM, &sig_act, NULL);

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
    if(*num_threads < MAX_NUM_THREADS)
    {
        //atomic_fetch_add(num_threads, 1);
	num_threads++;
    }
    else
        return -1;

    gtthread *new_gtthread = malloc(sizeof(gtthread*));
    
    *thread = generate_thread_id();                   
    new_gtthread->thread_id = *thread;
    new_gtthread->status = ACTIVE;                    // Need to decide what status to give it
    new_gtthread->retval = NULL;
    insert_thread_list(gtthread_head, gtthread_tail, new_gtthread);                            // Implement enqueue, dequeue, head and tail
    
    ucontext_t new_thread;
    getcontext(&new_thread);
    
    new_thread.uc_link = 0;
    new_thread.uc_stack.ss_sp = malloc(STACK_SIZE);
    new_thread.uc_stack.ss_size = STACK_SIZE;
    new_thread.uc_stack.ss_flags = 0;
    makecontext(&new_thread, gtthread_run, 2, start_routine, arg);  

    Qnode *new_node = malloc(sizeof(Qnode*));
    new_node->context = new_thread;
    new_node->thread_id = *thread;
    new_node->next = NULL;

    retval = enqueue_sched(scheduler_head, scheduler_tail, new_node);
    if(retval < 0)
        return -1;
}


int  gtthread_join(gtthread_t thread, void **status)
{
    if(gtthread_equal(gtthread_self(), thread) > 0)
        return -1;

    // ....
}

void gtthread_exit(void *retval)
{
    //atomic_fetch_sub(num_threads, 1);
    num_threads--;

    gtthread_t thread_id = current_Qnode->thread_id;
    gtthread *ptr = search_thread_list(thread_id);

    if(ptr == NULL)
        //do something here
        return;
    else
    {
        ptr->status = FINISHED;
        ptr->retval = retval;
    }
    //free Qnode - make sure its not used again till it's set
    free(current_Qnode);
}


int  gtthread_yield(void)
{
    //reset timer
    //call scheduler - put at end of queue
    setitimer(ITIMER_VIRTUAL, &timer, NULL);          
    gtthread_scheduler(0);          //what int to pass here - maybe set the timer from scheduler based on what we pass here

    //return stuff
}

int  gtthread_equal(gtthread_t t1, gtthread_t t2)
{
    if(t1 == t2)
        return 0;           //check what pthread_equal returns
    else
        return -1;
}

/*
int  gtthread_cancel(gtthread_t thread);
*/

gtthread_t gtthread_self(void)
{
    if(current_Qnode == NULL)
        return -1;
    
    return current_Qnode->thread_id;
}
/*

int  gtthread_mutex_init(gtthread_mutex_t *mutex);
int  gtthread_mutex_lock(gtthread_mutex_t *mutex);
int  gtthread_mutex_unlock(gtthread_mutex_t *mutex);
*/

int gtthread_run(void* (*start_routine)(void*), void *arg)
{
    void *retval;
    //need to set retval somehow
    start_routine(arg);          //check
    gtthread_exit(retval);
}

gtthread_t generate_thread_id()
{
    gtthread_t thread_id;
    gtthread *ptr;

    srand(time(NULL)); 
    while(1)
    {
        thread_id = (unsigned long) rand();           // try making a long random number
        if(search_thread_list(thread_id) == NULL)
             return thread_id;
    } 
}

void gtthread_scheduler(int signum)
{
    Qnode *next_Qnode;
    next_Qnode = dequeue_sched(scheduler_head, scheduler_tail);
    // enqueue current_Qnode
    enqueue_sched(scheduler_head, scheduler_tail, current_Qnode);
    // Set current_Qnode to next_Qnode
    current_Qnode = next_Qnode;
    // switch context
    if(setcontext(&current_Qnode->context) < 0)                //if there is an error - decide what to do here
        exit;
}

void main()
{
    return;
}
