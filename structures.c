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