/*
* C program to create a child proces and increment or decrement a variable.
* Avery Briggs
* 3471065
* Set.25/2018
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int num = 0;


void * foo(){
	printf("pthread id: %lld\n", (long long)pthread_self());
	pthread_exit(0);
}


int main(int argc, char ** argv){
	int pid = fork();
	if(pid < 0){
		exit(-1);
	}
	else if(pid == 0){
		num++;
		printf("Child id: %d, num: %d\n", getpid(), num);
		sleep(2);
	}
	else{
		num--;
		printf("Parent id: %d, num: %d\n", getpid(), num);
		sleep(2);
	}
	return 0;
}
