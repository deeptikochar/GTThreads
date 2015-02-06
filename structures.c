#include "structures.h"

int enqueue_sched(Qnode *new_node)
{
    new_node->next = NULL;
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
    scheduler_tail = new_node;
    return 0;
}

Qnode* dequeue_sched()
{
    struct Qnode *ptr = scheduler_head;
    if(ptr == NULL)
        return NULL;
    if(scheduler_head == scheduler_tail)
    {
    	scheduler_head = NULL;
    	scheduler_tail = NULL;
        return ptr;
    }
    scheduler_head = scheduler_head->next;
    return ptr;
}

int delete_from_queue_sched(gtthread_t thread_id)
{
    struct Qnode *ptr = scheduler_head;
    struct Qnode *to_be_deleted;
    int retval = -1;

  //  printf("In delete_from_queue_sched\n");
    if(scheduler_head->thread_id == thread_id)
    {
        scheduler_head = NULL;
        if(scheduler_tail == scheduler_head)
            scheduler_tail = NULL;
        free(ptr);
        return 0;
    }
    while(ptr->next != NULL)
    {
        if(ptr->next->thread_id == thread_id)
        {
    //        printf("Found thread %lu\n", thread_id );
            retval = 0;
            break;
        }
        ptr = ptr->next;
    }
    if(retval == 0)
    {
        to_be_deleted = ptr->next;
        ptr->next = to_be_deleted->next;
        if(to_be_deleted == scheduler_tail)
            scheduler_tail = NULL;
        free(to_be_deleted);
    }
    return retval;
}

void insert_thread_list(gtthread *thread)
{
	thread->next = NULL;
 //   printf("a\n");
	if(gtthread_head == NULL)
	{
		gtthread_head = thread;                                                                                                                                  
		gtthread_tail = thread;
//        printf("%lu\n %p\n", gtthread_head->thread_id, gtthread_head);
		return;
	}
	gtthread_tail->next = thread;                                                                                                                                              
	gtthread_tail = thread;
 //   printf("%lu\n", gtthread_tail->thread_id);
}

int if_exists_thread_id(gtthread_t thread_id)
{
    struct gtthread *ptr = gtthread_head;
    while(ptr != NULL)
    {
        if(ptr->thread_id == thread_id)
            return 1;
        ptr = ptr->next;
    }
    return 0;
}

gtthread* search_thread_list(gtthread_t thread_id)
{
	gtthread *ptr = gtthread_head;
    while(ptr != NULL)
    {
        if(ptr->thread_id == thread_id)
            return ptr;
        ptr = ptr->next;
    }
    return NULL;
}


void print_scheduler_Q()
{
    printf("Printing scheduler queue\n");
    struct Qnode *ptr = scheduler_head;
    while(ptr != NULL)
    {
        printf("%lu\n", ptr->thread_id);
        ptr = ptr->next;
    }
}
void print_thread_list()
{
    printf("Printing thread list\n");
    struct gtthread *ptr = gtthread_head;
    while(ptr != NULL)
    {
        printf("%lu status: %d\n", ptr->thread_id, ptr->status);
        ptr = ptr->next;
    }
}