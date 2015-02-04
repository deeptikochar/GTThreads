#include "structures.h"

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
    new_node->next = NULL;
    scheduler_tail->next = new_node;
    scheduler_tail = new_node;
    return 0;
}

Qnode* dequeue_sched()
{
    Qnode *ptr = scheduler_head;
    if(ptr == NULL)
        return NULL;
    if(scheduler_head == scheduler_tail)
    {
    	scheduler_head = NULL;
    	scheduler_tail = NULL;
    }
    scheduler_head = scheduler_head->next;
    return ptr;
}

void insert_thread_list(gtthread *thread)
{
	thread->next = NULL;
	if(gtthread_head == NULL)
	{
		gtthread_head = thread;
		gtthread_tail = thread;
		return;
	}
	gtthread *ptr = gtthread_head;
	while(ptr->next == NULL)
	{
		ptr = ptr->next;
	}
	ptr->next = thread;
	gtthread_tail = thread;
}

gtthread* search_thread_list(gtthread_t thread_id);
