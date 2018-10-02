/*
*	CS3413 Lab2
*	Oct. 2 2018
*	Avery Briggs
*	3471065
*
*	C program to read in a line of text.
*	and perform basic operations on each 
*	character using pthreads. the char is
*	passed around using a pipe 
*
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	
int pipeEnd[2];

void * shift_lower_case(void * arg){
	char a;
	read(pipeEnd[0], &a, 1);
	char * ptr = &a;
	//printf("shift_lower_case(%d)\n", *ptr);
	if(*ptr > 64 && *ptr < 91){
		*ptr = *ptr +32;
		//printf("lower: %c\n", *ptr);
	}
	write(pipeEnd[1], ptr, 1);
	pthread_exit(0);
}

void * shift_5_right(void * arg){
	//printf("shift_5_right\n");
	char a;
	read(pipeEnd[0], &a, 1);
	char * ptr = &a;
	//printf("ptr: %c\n", *ptr);
	*ptr = *ptr +5;
	//printf("right: %c\n", *ptr);
	write(pipeEnd[1], ptr, 1);
	pthread_exit(0);
}

void * check_overflow(void * arg){
	//printf("check_overflow\n");
	char a;
	read(pipeEnd[0], &a, 1);
	char * ptr = &a;
	//printf("ptr: %c\n", *ptr);
	if(*ptr > 122){
		*ptr = *ptr -26;
		//printf("overflow: %c\n", *ptr);
	}
	write(pipeEnd[1], ptr, 1);
	pthread_exit(0);
}

int main(int argc, char ** argv){
	int count = 0;
	char a = 'a';
	char * buf = &a;
	char string[50];
	char * sptr = string;
	scanf("%c", buf);
	pthread_t pid1, pid2, pid3;
	pipe(pipeEnd);while(*buf != 10){ //looping until '\n' is read
		pthread_create(&pid1, NULL, shift_lower_case, NULL);
		write(pipeEnd[1], buf, 1);
		pthread_join(pid1, NULL);
		pthread_create(&pid2, NULL, shift_5_right, NULL);
		pthread_join(pid2, NULL);
		pthread_create(&pid3, NULL, check_overflow, NULL);
		pthread_join(pid3, NULL);
		read(pipeEnd[0], buf, 1);
		*sptr = *buf;
		printf("%c", *buf);
		sptr++;
		scanf("%c", buf);
		count++;
	}
	close(pipeEnd[0]);
	close(pipeEnd[1]);
	printf("\n");
	return EXIT_SUCCESS;
}
