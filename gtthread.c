#include "gtthread.h"

int num_threads = 0;
struct sigaction sig_act;
struct itimerval timer;
sigset_t mask;

struct Qnode *scheduler_head = NULL;
struct Qnode *scheduler_tail = NULL;
struct Qnode *current_Qnode = NULL;
struct gtthread *gtthread_head = NULL;
struct gtthread *gtthread_tail = NULL;


void mock_scheduler(int signum);

void gtthread_init(long period)
{
    ucontext_t current;
    struct Qnode *initial_node = malloc(sizeof(Qnode));
    struct gtthread *initial_thread = malloc(sizeof(gtthread));

    getcontext(&current);
    
    
    initial_node->context = current;
    initial_node->next = NULL;
    initial_node->thread_id = generate_thread_id();

    
    initial_thread->thread_id = initial_node->thread_id;
    initial_thread->status = ACTIVE;
    initial_thread->retval = NULL;

    insert_thread_list(initial_thread);

    current_Qnode = initial_node;



    
    /* Setting the timer */

    memset(&sig_act, 0, sizeof(sig_act));
    sig_act.sa_handler = &gtthread_scheduler;         
    sigaction(SIGVTALRM, &sig_act, NULL);

    sigemptyset(&mask);
    sigaddset(&mask, SIGVTALRM);

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = period;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = period;
    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    return;
}

int  gtthread_create(gtthread_t *thread,
                     void *(*start_routine)(void *),
                     void *arg)
{
    int retval = 0;
    ucontext_t new_thread;
    struct gtthread *new_gtthread = malloc(sizeof(gtthread));
    struct Qnode *new_node = malloc(sizeof(Qnode));

    block_signal();
    if(num_threads < MAX_NUM_THREADS)
    {
	   num_threads++;
    }
    else
        return -1;

    
    *thread = generate_thread_id();   

    new_gtthread->thread_id = *thread;

    new_gtthread->status = ACTIVE;                    
    new_gtthread->retval = NULL;
    
    getcontext(&new_thread);    
    new_thread.uc_link = 0;
    new_thread.uc_stack.ss_sp = malloc(STACK_SIZE);
    new_thread.uc_stack.ss_size = STACK_SIZE;
    new_thread.uc_stack.ss_flags = 0;
    makecontext(&new_thread, (void (*)(void)) gtthread_run, 2, start_routine, arg);  
    if(retval < 0)
        return retval;
    
    new_node->context = new_thread;
    new_node->thread_id = *thread;
    new_node->next = NULL;

    insert_thread_list(new_gtthread);
    retval = enqueue_sched(new_node);
    unblock_signal();

    return retval;
}


int  gtthread_join(gtthread_t thread, void **status)
{
    struct gtthread* to_be_joined;
    if(status != NULL)
        *status = NULL;
    if(gtthread_equal(gtthread_self(), thread) > 0)
        return -1;

    to_be_joined = search_thread_list(thread);
    if(to_be_joined == NULL)
        return -1;
    while(1)
    {
        if(to_be_joined->status == FINISHED || to_be_joined->status == CANCELLED)
        {
            if(status != NULL)
                *status = to_be_joined->retval;
            return 0;
        }
        gtthread_yield();
    }
}

void gtthread_exit(void *retval)
{
    gtthread_t thread_id;
    struct gtthread *ptr;

    block_signal();
    num_threads--;

    thread_id = gtthread_self();
    ptr = search_thread_list(thread_id);

    if(ptr == NULL)
        return;
    else
    {
        ptr->status = FINISHED;
        ptr->retval = retval;
    }

    free(current_Qnode);
    gtthread_scheduler(-1);
}


int  gtthread_yield(void)
{         
    gtthread_scheduler(0); 
    return 0;

}

int  gtthread_equal(gtthread_t t1, gtthread_t t2)
{
    if(t1 == t2)
        return 1;           
    else
        return 0;
}


int  gtthread_cancel(gtthread_t thread)
{
    int retval = 0;
    struct gtthread *ptr;

    block_signal();
    
    num_threads--;
    ptr = search_thread_list(thread);

    
    if(ptr == NULL)
    {
        printf("This thread doesn't exist\n");
        unblock_signal();
        return -1;
    }
    else if(ptr->status == CANCELLED)
    {
        printf("This thread has already been cancelled\n");
        unblock_signal();
        return -1;
    }
    else if(ptr->status == FINISHED)
    {
        printf("This thread has already finished\n");
        unblock_signal();
        return -1;
    }
    ptr->status = CANCELLED;
    ptr->retval = ((void *)(size_t) -1);

    if(thread == gtthread_self())
    {
        free(current_Qnode);
        gtthread_scheduler(-1);
        return 0;
    }

    retval = delete_from_queue_sched(thread);
    unblock_signal();
    return retval;
}


gtthread_t gtthread_self(void)
{
    if(current_Qnode == NULL)
        return -1;
    
    return current_Qnode->thread_id;
}


void gtthread_run(void* (*start_routine)(void*), void *arg)
{

    void *retval;
    retval = (void *) start_routine(arg);          
    gtthread_exit(retval);
    return;
}

gtthread_t generate_thread_id()
{
    gtthread_t thread_id;
    block_signal();
    srand(time(NULL)); 
    
    while(1)
    {
        thread_id = (unsigned long) rand();          
        if(!if_exists_thread_id(thread_id))
        {
            unblock_signal();
            return thread_id;
        }
    } 
}
