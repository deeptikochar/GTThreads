#include "structures.h"

int enqueue_sched(Qnode *head, Qnode *tail, Qnode *new_node)
{
    if(tail == NULL)
    {
        if(head == NULL)
        {
            head = new_node;
            tail = new_node;
            return 0;
        }    
        return -1;
    }
    new_node->next = NULL;
    tail->next = new_node;
    tail = new_node;
    return 0;
}

Qnode* dequeue_sched(Qnode *head, Qnode *tail)
{
    Qnode *ptr = head;
    if(ptr == NULL)
        return NULL;
    if(head == tail)
    {
    	head = NULL;
    	tail = NULL;
    }
    head = head->next;
    return ptr;
}

void insert_thread_list(gtthread *head, gtthread *tail, gtthread *thread)
{
	thread->next = NULL;
	if(head == NULL)
	{
		head = thread;
		tail = thread;
		return;
	}
	gtthread *ptr = head;
	while(ptr->next == NULL)
	{
		ptr = ptr->next;
	}
	ptr->next = thread;
	tail = thread;
}

gtthread* search_thread_list(gtthread_t thread_id)
{
	gtthread *something;
	return something;
}
