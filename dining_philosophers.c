#include <stdint.h>
#include "gtthread.h"

#define NUM_CHOPSTICKS 5
#define MAX_DURATION 100000000

gtthread_mutex_t chopstick_mutex[NUM_CHOPSTICKS];



void think(int duration, int phil_num)
{
	int i;
	printf("Philosopher %d is thinking\n", phil_num);
	for(i = 0; i < duration; i++);
}

void eat(int duration, int phil_num)
{
	int i;
	int index1 = phil_num - 1;
	int index2 = phil_num % NUM_CHOPSTICKS;
	printf("Philosopher %d is hungry\n", phil_num);	

	if(index1 < index2)
	{
		gtthread_mutex_lock(&chopstick_mutex[index1]);
		printf("Philosopher %d has picked up left chopstick\n", phil_num);
		gtthread_mutex_lock(&chopstick_mutex[index2]);
		printf("Philosopher %d has picked up right chopstick\n", phil_num);
	}
	else
	{
		gtthread_mutex_lock(&chopstick_mutex[index2]);
		printf("Philosopher %d has picked up right chopstick\n", phil_num);
		gtthread_mutex_lock(&chopstick_mutex[index1]);
		printf("Philosopher %d has picked up left chopstick\n", phil_num);
	}

	printf("Philosopher %d is eating\n", phil_num);
	for(i = 0; i < duration; i++);

	gtthread_mutex_unlock(&chopstick_mutex[index1]);
	printf("Philosopher %d has released left chopstick\n", phil_num);
	gtthread_mutex_unlock(&chopstick_mutex[index2]);	
	printf("Philosopher %d has released right chopstick\n", phil_num);

	return;
}

void* philosopher(void *phil_num)
{
	int duration;
	srand(time(NULL));

	while(1)
	{
		duration = rand() % MAX_DURATION;           
		think(duration, (int) (intptr_t) phil_num);
		duration = rand() % MAX_DURATION;
		eat(duration, (int) (intptr_t) phil_num);
	}
	return NULL;
}

int main()
{
	int i;
	long period = 1000;
	gtthread_t philosopher_thread[NUM_CHOPSTICKS];
	gtthread_init(period);
	

	for (i = 0; i < NUM_CHOPSTICKS; i++)
	{
		gtthread_mutex_init(&chopstick_mutex[i]);
	}

	for(i = 0; i < NUM_CHOPSTICKS; i++)
	{

		if(gtthread_create(&philosopher_thread[i], philosopher,(void*) (intptr_t) (i+1) ) < 0)
			return -1;
	}
	
	gtthread_join(philosopher_thread[0], NULL);
	return 0;
}
