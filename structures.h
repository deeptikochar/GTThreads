
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


int enqueue_sched(Qnode *head, Qnode *tail, Qnode *new_node);
Qnode* dequeue_sched(Qnode *head, Qnode *tail);

void insert_thread_list(gtthread **head, gtthread **tail, gtthread **thread);
gtthread* search_thread_list(gtthread *head, gtthread_t thread_id);        //Returns ptr if the thread_id is present, NULL if not
int f_exists_thread_id(gtthread *head, gtthread_t thread_id);


void print_scheduler_Q(Qnode *head);
void print_thread_list(gtthread *head);
