#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_LOOPS 5000000

long long sum = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * counting_threads(void * arg){
	int offset = *((int *)arg);
	int i;
	for(i = 0; i < NUM_LOOPS; i++){
		pthread_mutex_lock(&mutex);
		sum += offset;
		pthread_mutex_unlock(&mutex);
	}
	pthread_exit(NULL);
}

int main(int argc, char ** argv){
	pthread_t pid, tid;
	int offset1 = 1, offset2 = -1;
	pthread_create(&pid, NULL, counting_threads, &offset1);
	pthread_create(&tid, NULL, counting_threads, &offset2);
	pthread_join(pid, NULL);
	pthread_join(tid, NULL);
	printf("sum: %lld\n", sum);
	return 0;
}
