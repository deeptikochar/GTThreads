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

int if_exists_thread_id(gtthread *head, gtthread_t thread_id)
{
    struct gtthread *ptr = head;
    while(ptr != NULL)
    {
        if(ptr->thread_id == thread_id)
            return 1;
        ptr = ptr->next;
    }
    return 0;
}

gtthread* search_thread_list(gtthread *head, gtthread_t thread_id)
{
	gtthread *ptr;
}


void print_scheduler_Q(Qnode *head)
{
    struct gtthread *ptr = head;
    while(ptr != NULL)
    {
        printf("%lu\n", ptr->thread_id);
        ptr = ptr->next;
    }
}
void print_thread_list(gtthread *head)
{
    struct gtthread *ptr = head;
    while(ptr != NULL)
    {
        printf("%lu status: %d\n", ptr->thread_id, ptr->status);
        ptr = ptr->next;
    }
}