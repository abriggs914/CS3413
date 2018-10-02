#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	
int pipeEnd1[2];
int pipeEnd2[2];
int pipeEnd3[2];
int pipeEnd4[2];

void * shift_lower_case(void * arg){
	return NULL;
}

void * shift_5_right(void * arg){
	return NULL;}

void * check_overflow(void * arg){
	return NULL;}

int main(int argc, char ** argv){
	char c = '1';
	char * buf = &c;
	scanf("%c", buf);
	printf("buf: %c\n", *buf);
	while(*buf != '\n'){
		pipe(pipeEnd1);
		if(fork() != 0){
			close(pipeEnd1[0]);
			write(pipeEnd1[1], buf, 1);
			close(pipeEnd1[1]);
			wait(NULL);
			printf("fork == 0 b, buf: %c\n", *buf);
			char p = p;
			buf = &p;
			printf("fork == 0 a, buf: %c\n", *buf);
		}
		else{
			close(pipeEnd1[1]);
			read(pipeEnd1[0], buf, 1);
			close(pipeEnd1[0]);
			
			printf("fork == 1 b, buf: %c\n", *buf);
		}
		*buf = '\n';
	}
	
	return EXIT_SUCCESS;
}
