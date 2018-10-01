#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include  <sys/types.h>
#include <unistd.h> 

long long sum = 0;
int num = 0;
#define   MAX_COUNT  200
#define   BUF_SIZE   100

void * sum_runner(void * arg){
	//fork();
	//int pid = getpid();
	long long * limit_ptr = (long long *) arg;
	long long limit = *limit_ptr;
	long long i;
	for(i = 0; i <= limit; i++){
		sum += i;
	}
	pthread_exit(0);
}

void * foo(){
	fork();
	int pid = getpid();
	if(pid == 0){
		num++;
	}
	else{
		num--;
	}
	printf("Num: %d, by pid: %d\n", num, pid);
	pthread_exit(0);
}


int main(int argc, char ** argv){
	if(argc < 2){
		printf("Usage: %s <nums>\n", argv[0]);
		exit(-1);
	}
	//long long limit = atoll(argv[1]);
	pthread_t mythread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&mythread, NULL, foo, NULL);
	
	fork();
	int pid = getpid();
	printf("%d\n", pid);
	//long long i;
    /*for (i = 1; i <= MAX_COUNT; i++) {			
    	printf("This line is from pid %d, value = %lld\n", pid, i);
  	}*/ 
	pthread_join(mythread, NULL);
	printf("Sum is: %lld\n", sum);
	return 0;
}
