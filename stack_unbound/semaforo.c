#include<pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>

/* Producer/consumer program illustrating conditional variables */

/* Size of shared buffer */
#define BUF_SIZE 3

int buffer[BUF_SIZE];					/* shared buffer */
int add=0;										/* place to add next element */
int rem=0;										/* place to remove next element */
int num=0;										/* number elements in buffer */

sem_t s_lugares;
sem_t s_ocupados;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Produce value(s) */
void *producer(void *param)
{
	int i;
	for (i=1; i<=20; i++) {
        sem_wait(&s_lugares);
        pthread_mutex_lock(&mutex); 
		/* if executing here, buffer not full so add element */
		buffer[add] = i;
		add = (add+1) % BUF_SIZE;
		num++;
        printf ("producer: inserted %d\n", i);  fflush (stdout);

        pthread_mutex_unlock(&mutex); 
        sem_post(&s_ocupados);
	}

	printf ("producer quiting\n");  fflush (stdout);
}

/* Consume value(s); Note the consumer never terminates */
void *consumer(void *param)
{
	int i;
	while (1) {
        sem_wait(&s_ocupados);
        pthread_mutex_lock(&mutex); 
		/* if executing here, buffer not empty so remove element */
		i = buffer[rem];
		rem = (rem+1) % BUF_SIZE;
		num--;
        printf ("Consume value %d\n", i);  fflush(stdout);

        sem_post(&s_lugares);
        pthread_mutex_unlock(&mutex); 
	}
}

int main (int argc, char *argv[]){
	pthread_t tid1, tid2;		/* thread identifiers */

    sem_init(&s_lugares, 0, BUF_SIZE);
    sem_init(&s_ocupados, 0, 0);

	int i;

	/* create the threads; may be any number, in general */
	if (pthread_create(&tid1,NULL,producer,NULL) != 0) {
		fprintf (stderr, "Unable to create producer thread\n");
		exit (1);
	}
	if (pthread_create(&tid2,NULL,consumer,NULL) != 0) {
		fprintf (stderr, "Unable to create consumer thread\n");
		exit (1);
	}
	/* wait for created thread to exit */
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	printf ("Parent quiting\n");
}
