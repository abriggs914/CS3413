/*
* C program to create a thread and print it's id.
* Avery Briggs
* 3471065
* Set.25/2018
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void * foo(){
	printf("pthread id: %lld\n", (long long)pthread_self());
	pthread_exit(0);
}


int main(int argc, char ** argv){    
    
	pthread_t mythread;
	pthread_create(&mythread, NULL, &foo, NULL);
	pthread_join(mythread, NULL);
	return 0;
}
