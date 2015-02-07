# GTThreads
Advanced Operating Systems Project

Linux Platform: Ubuntu 14.04

Compilation:
In the main directory, type "make".

This will give gtthread.a and dining_philosophers.

Run:

To run the dining philosophers program type:
./dining_philosophers

Premptive scheduler implementation:

I have created a queue data structure which stores thread ids and contexts. When a thread is created or swapped out, it is inserted at the end of the queue. The thread at the start of the queue is dequeued and swapped in by the scheduler.

I've used setitimer to generate SIGVTALRM signals at time intervals specified by the input to gtthread_init. I have specified the function gtthread_scheduler as the signal handler. When a signal comes in, the scheduler is called. It dequeues the next thread and enqueues the current one. It then swaps the contexts of these threads.

The scheduler is also called with argument -1 when a thread exits. In this case, it does not enqueue the thread again. It also doesn't swapcontext. Instead it uses setcontext to set the context to the next thread's.


Dining Philosophers:

To implement the dining philosophers problem, I've used one mutex for each chopstick. The philosopher's eat and think for random amounts of time.

To prevent deadlock, each philosopher must first pick up the chopstick with the lower number. For all except one philosopher, this will be the left chopstick. Only the last philosopher will pick up his right chopstick first (his left chopstick is #5 while the right one is #1). This ensures that even if the first 4 philosopher's pick up their left chopsticks simultaneously, there will be one chopstick left on the table since this is the highest numbered chopstick. Therefore it can be picked up by the fourth philosopher who can eat and then release both his chopsticks which can be picked up by the next philosopher.


