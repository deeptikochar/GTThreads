
#include "gtthread.h"

#define NUM_CHOPSTICKS 5
#define MAX_DURATION 100000000

gtthread_mutex_t chopstick_mutex[NUM_CHOPSTICKS];
/*gtthread_mutex_t pick_up_chopsticks;*/


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

	/*
	while(1)
	{
		gtthread_mutex_lock(&pick_up_chopsticks);
		if(chopstick[index1] == 1 && chopstick[index2] == 1)
		{
			chopstick[index1] = 0;
			chopstick[index2] = 0;
			printf("Philosopher %d has picked up left chopstick\n", phil_num);
			printf("Philosopher %d has picked up right chopstick\n", phil_num);
			break;
		}
		gtthread_mutex_unlock(&pick_up_chopsticks);
		gtthread_yield();
	}
	

	gtthread_mutex_lock(&pick_up_chopsticks);
	chopstick[index1] = 1;
	chopstick[index2] = 1;
	printf("Philosopher %d has released left chopstick\n", phil_num);
	printf("Philosopher %d has released right chopstick\n", phil_num);
	gtthread_mutex_unlock(&pick_up_chopsticks);
	*/

	return;
}

void* philosopher(int phil_num)
{
	int duration;
	srand(time(NULL));
	
	/*wait for this duration before starting to eat */
	while(1)
	{
		duration = rand() % MAX_DURATION;
		think(duration, phil_num);
		duration = rand() % MAX_DURATION;
		eat(duration, phil_num);
	}
	return NULL;
}

int main()
{
	int i;
	long period = 1000;
	gtthread_init(period);
	gtthread_t philosopher_thread[NUM_CHOPSTICKS];
	/*gtthread_mutex_init(&pick_up_chopsticks);*/

	for (i = 0; i < NUM_CHOPSTICKS; i++)
	{
		gtthread_mutex_init(&chopstick_mutex[i]);
	}

	for(i = 0; i < NUM_CHOPSTICKS; i++)
	{

		if(gtthread_create(&philosopher_thread[i], philosopher, (i+1) ) < 0)
			return -1;
	}
	
	gtthread_join(philosopher_thread[0], NULL);
	return 0;
}
