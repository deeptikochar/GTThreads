
#include "gtthread.h"

typedef enum {ACTIVE, FINISHED} status_t;

struct gtthread
{
	gtthread_t thread_id;
	status_t status;
	void *retval;
	gtthread *next;
}

typedef struct Qnode
{
	ucontext_t context;
	gtthread_t thread_id;
	Qnode *next;
}

Qnode *scheduler_head = NULL;
Qnode *scheduler_tail = NULL;
gtthread *gtthread_head = NULL:
gtthread *gtthread_tail = NULL;

int enqueue_sched(Qnode *new_node);
Qnode* dequeue_sched();

void insert_thread_list(gtthread *thread);
gtthread* search_thread_list(gtthread_t thread_id);        //Returns ptr if the thread_id is present, NULL if not
