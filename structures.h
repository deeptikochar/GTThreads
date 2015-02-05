
#define gtthread_t unsigned long int


#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {ACTIVE, FINISHED} status_t;

typedef struct gtthread
{
	gtthread_t thread_id;
	status_t status;
	void *retval;
	struct gtthread *next;
} gtthread;


typedef struct Qnode
{
	ucontext_t context;
	gtthread_t thread_id;
	struct Qnode *next;
} Qnode;

extern struct gtthread *gtthread_head;
extern struct gtthread *gtthread_tail;
extern struct Qnode *scheduler_head;
extern struct Qnode *scheduler_tail;

int enqueue_sched(Qnode *new_node);
Qnode* dequeue_sched();

void insert_thread_list(gtthread *thread);
gtthread* search_thread_list(gtthread_t thread_id);        //Returns ptr if the thread_id is present, NULL if not
int if_exists_thread_id(gtthread_t thread_id);


void print_scheduler_Q();
void print_thread_list();
