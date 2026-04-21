#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Producer/consumer program illustrating conditional variables */

/* Size of shared buffer */
#define BUF_SIZE 3

int buffer[BUF_SIZE];					/* shared buffer */
int add=0;										/* place to add next element */
int rem=0;										/* place to remove next element */
int num=0;										/* number elements in buffer */
int flag = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_non_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_non_empty = PTHREAD_COND_INITIALIZER;

/* Produce value(s) */
void *producer(void *param)
{
	int i;
	for (i=1; i<=20; i++) {
        pthread_mutex_lock(&mutex);
        
        while(num == BUF_SIZE){
            pthread_cond_wait(&cond_non_full, &mutex);
        }

		/* if executing here, buffer not full so add element */
		buffer[add] = i;
		add = (add+1) % BUF_SIZE;
		num++;
        
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_non_empty);
        printf ("producer: inserted %d\n", i);  fflush (stdout);
	}
	printf ("Producer quiting\n");  fflush (stdout);
    flag = 0;
    return NULL;
}

/* Consume value(s); Note the consumer never terminates */
void *consumer(void *param)
{
	int i;
	while (flag) {
        pthread_mutex_lock(&mutex);
        while(num == 0){
            pthread_cond_wait(&cond_non_empty, &mutex);
        }
    
		/* if executing here, buffer not empty so remove element */
		i = buffer[rem];
		rem = (rem+1) % BUF_SIZE;
		num--;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_non_full);
        printf ("Consume value %d\n", i);  fflush(stdout);
	}
	printf ("Consumer quiting\n");  fflush (stdout);
    return NULL;
}

int main (int argc, char *argv[])
{
	pthread_t tid1, tid2;		/* thread identifiers */
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
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	printf ("Parent quiting\n");
    return 0;
}
