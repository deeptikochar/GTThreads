# GTThreads
Advanced Operating Systems Project

Name: Deepti Kochar

Linux Platform: Ubuntu 14.04

Compilation:
In the top directory, type: 
make

This will give gtthread.a and dining_philosophers.
To compile a test case, type:
gcc -Wall -pedantic -I. -o main main.c gtthread.a

Run:

To run the dining philosophers program type:
./dining_philosophers

Preemptive scheduler implementation:

I have created a queue data structure which stores thread ids and contexts. When a thread is created or swapped out, it is inserted at the end of the queue. The thread at the start of the queue is dequeued and swapped in by the round-robin scheduler.

I've used setitimer to generate SIGVTALRM signals at time intervals specified by the input to gtthread_init. I have specified the function gtthread_scheduler as the signal handler. When a signal comes in (preemption), the scheduler is called. It dequeues the next thread and enqueues the current one in the queue data structure. It then swaps the contexts of these threads using swapcontext.

The scheduler is also called with argument -1 when a thread exits or cancels itself. In this case, it does not enqueue the thread again. It also doesn't use swapcontext (since we don't need to store this thread's context any longer). Instead it uses setcontext to set the context to the next thread's.

When a thread calls gtthread_yield, the scheduler is called and it swaps out this thread and swaps in the next in the queue.


Dining Philosophers Solution:

To implement the dining philosophers problem, I've used one mutex for each chopstick. The philosopher's eat and think for random amounts of time (implemented by generating a random integer and then looping that many times).

There are 5 philosophers (Philosopher #1 to #5) and 5 chopsticks (chopstick #1 to #5). Chopstick 1 is on the left of Philosopher 1. So he must pick up chopstick 1 and 2 to eat. Similarly for philosophers 2-4. Philosopher 5 must pick up chopstick 5 and 1 to eat.

To prevent deadlocks, each philosopher must first pick up the chopstick with the lower number. For all except the last philosopher, this will be the left chopstick. Only the last philosopher will pick up his right chopstick first (his left chopstick is #5 while the right one is #1). This ensures that even if the first 4 philosophers pick up their left chopsticks simultaneously, there will be one chopstick (#5) left on the table since this is the highest numbered chopstick. Therefore it can be picked up by the fourth philosopher who can eat and then release both his chopsticks which can be picked up by the next philosopher.


Thoughts:

This project was a great learning experience. It helped me understand a lot about how threads can be implemented, how a scheduler must work. While studying these topics, we usually learn the policies which must be followed but not how they are implemented. Starting the project was difficult, but once I understood some of the key concepts, the rest came intuitively.

This implementation does not handle the case where 2 threads call gtthread_join on each other.
