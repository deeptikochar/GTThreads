#include "gtthread.h"

//_Atomic(int) *num_threads = 0;
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
 //   printf("Enqueuing stuff\n");
    ucontext_t current;
    getcontext(&current);
    
    struct Qnode *initial_node = malloc(sizeof(Qnode));
    initial_node->context = current;
    initial_node->next = NULL;
    initial_node->thread_id = generate_thread_id();

    struct gtthread *initial_thread = malloc(sizeof(gtthread));
    initial_thread->thread_id = initial_node->thread_id;
    initial_thread->status = ACTIVE;
    initial_thread->retval = NULL;

//    printf("%lu\n", initial_thread->thread_id);

    insert_thread_list(initial_thread);
//    enqueue_sched(initial_node);   - don't enqueue the current thread
    current_Qnode = initial_node;

//    printf("%p\n", gtthread_head);

    
    // Setting the timer
//    printf("Setting the timer\n");
    memset(&sig_act, 0, sizeof(&sig_act));
    sig_act.sa_handler = &gtthread_scheduler;         //change to actual scheduler
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
    struct gtthread *new_gtthread = malloc(sizeof(gtthread));
    ucontext_t new_thread;
    struct Qnode *new_node = malloc(sizeof(Qnode));

    //The value can change after reading - need to fix this. Disable context switch
    block_signal();
    if(num_threads < MAX_NUM_THREADS)
    {
        //atomic_fetch_add(num_threads, 1);
	num_threads++;
    }
    else
        return -1;

    
    *thread = generate_thread_id();   
    unblock_signal();

    new_gtthread->thread_id = *thread;
 //   printf("%lu\n", new_gtthread->thread_id);
    new_gtthread->status = ACTIVE;                    // Need to decide what status to give it
    new_gtthread->retval = NULL;
    
    getcontext(&new_thread);    
    new_thread.uc_link = 0;
    new_thread.uc_stack.ss_sp = malloc(STACK_SIZE);
    new_thread.uc_stack.ss_size = STACK_SIZE;
    new_thread.uc_stack.ss_flags = 0;
    makecontext(&new_thread, (void *) gtthread_run, 2, start_routine, arg);  
    if(retval < 0)
        return retval;
    
    new_node->context = new_thread;
    new_node->thread_id = *thread;
    new_node->next = NULL;

    insert_thread_list(new_gtthread);
    retval = enqueue_sched(new_node);

    return retval;
}


int  gtthread_join(gtthread_t thread, void **status)
{
    if(status != NULL)
        *status = NULL;
    if(gtthread_equal(gtthread_self(), thread) > 0)
        return -1;

    // check the status. if it is finished get the return value
    // if it is active, wait
    //if it is cancelled print and return -1
    struct gtthread* to_be_joined = search_thread_list(thread);
    if(to_be_joined == NULL)
        return -1;
    while(1)
    {
        if(to_be_joined->status == FINISHED)
        {
            if(status != NULL)
                *status = to_be_joined->retval;
            return 0;
        }
        if(to_be_joined->status == CANCELLED)
            return -1;
        gtthread_yield();
    }
}

void gtthread_exit(void *retval)
{
    // might need to block signals
    //atomic_fetch_sub(num_threads, 1);
    block_signal();
//    printf("In gtthread_exit\n");
    num_threads--;

    gtthread_t thread_id = current_Qnode->thread_id;
    struct gtthread *ptr = search_thread_list(thread_id);

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
    unblock_signal();
    //call scheduler - make sure it doesn't use current_Qnode
    gtthread_scheduler(-1);
}


int  gtthread_yield(void)
{
    //reset timer
    //call scheduler - put at end of queue
 //   setitimer(ITIMER_VIRTUAL, &timer, NULL);          
    gtthread_scheduler(0);          //what int to pass here - maybe set the timer from scheduler based on what we pass here

    //return stuff
}

int  gtthread_equal(gtthread_t t1, gtthread_t t2)
{
    if(t1 == t2)
        return 1;           //check what pthread_equal returns
    else
        return 0;
}


int  gtthread_cancel(gtthread_t thread)
{
    //might need to block signals
    block_signal();
    int retval = 0;
    num_threads--;
    struct gtthread *ptr = search_thread_list(thread);

    if(thread == gtthread_self())
    {
        printf("Cannot cancel self\n");
        unblock_signal();
        return -1;
    }
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
    //ptr->retval = -1;

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


int gtthread_run(void* (*start_routine)(void*), void *arg)
{
//    printf("In gtthread_run\n");
    void *retval;
    //need to set retval somehow
    retval = (void *) start_routine(arg);          //check
    gtthread_exit(retval);
    return 0;
}

gtthread_t generate_thread_id()
{
    gtthread_t thread_id;
    block_signal();
    srand(time(NULL)); 
    
    while(1)
    {
        thread_id = (unsigned long) rand();           // try making a long random number
        if(!if_exists_thread_id(thread_id))
        {
            unblock_signal();
            return thread_id;
        }
    } 
}








int shared = 0;
struct gtthread_mutex_t mutex;

void mock_scheduler(int signum)
{
 //   printf("This is a new thread\n");
    int i, j, k;
    for (i = 0; i < 100; i++)
    {
        for(j = 0; j < 100; j++)
        {
            gtthread_mutex_lock(&mutex);
            shared = 2;
            gtthread_mutex_unlock(&mutex);
        }
    }
}

void main()
{
    printf("in main\n");
    //gtthread_t thread = generate_thread_id();
    //printf("%lu\n", thread );
    int i = 1;
    int j;
    printf("%d\n",i++);
    gtthread_init(500000);
    
    gtthread_mutex_init(&mutex);
    printf("%d\n",i++);
    gtthread_t new_one;
    int *ret;
    gtthread_create(&new_one, (void *) &mock_scheduler, (void *) ret);
    print_scheduler_Q();
    print_thread_list();
 //   struct Qnode *node = dequeue_sched();
   // print_scheduler_Q();
 //   block_signal();
 //   struct Qnode *next = dequeue_sched();
   // struct  Qnode *prev = current_Qnode;

//    if(next == NULL || prev == NULL)
 //       printf("a pointer is null");
  //  if(swapcontext(&(prev->context), &(next->context)) < 0)
   //      printf("not swapped\n");

 //   printf("swapped\n");

    for(i=0; i< 100; i++)
    {
        for(j=0; j< 1000;j++)
        {
            gtthread_mutex_lock(&mutex);
            shared = 1;
            gtthread_mutex_unlock(&mutex);
        }
    }
    void *status;
    int retval = gtthread_join(new_one, status);
    printf("joined with %d\n", retval);

//    unblock_signal();

    printf("%d", shared);
    return;
}

