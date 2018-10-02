#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	
int pipeEnd[2];

void * shift_lower_case(void * arg){
	//char * ptr = NULL;
	//ptr = (char *)line;
	//dup2(pipeEnd[0], STDIN_FILENO);
	printf("shift_lower_case\n");
	pthread_mutex_lock(&mutex);	
	int l = read(pipeEnd[0], arg, 100);
	pthread_mutex_unlock(&mutex);
	printf("l: %d\n", l);
	char c = 'z';
	char * ptr = (char *)arg;
	printf("shift_lower_case(%c)\n", *ptr);
	if(*ptr > 64 && *ptr < 91){
		*ptr = *ptr +32;
		printf("*ptr: %s\n", ptr);
	}
	pthread_mutex_lock(&mutex);	
	pipe(pipeEnd);
	close(pipeEnd[0]);
	write(STDOUT_FILENO, ptr, 1);
	close(pipeEnd[1]);
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

void * shift_5_right(void * arg){
	printf("shift_5_right\n");
	pthread_mutex_lock(&mutex);
	int l = read(pipeEnd[0], arg, 1);
	pthread_mutex_unlock(&mutex);
	printf("l: %d\n", l);
	char c = 'a';
	char * ptr = (char *)arg;
	*ptr = *ptr +5;
	printf("*ptr: %s\n", ptr);
	pthread_exit(0);
}

void * check_overflow(void * arg){
	printf("check_overflow\n");
	pthread_mutex_lock(&mutex);
	int l = read(pipeEnd[0], arg, 100);
	pthread_mutex_unlock(&mutex);
	printf("l: %d\n", l);
	char c = 'a';
	char * ptr = &c;
	if(*ptr > 122){
		*ptr = *ptr -26;
	}
	pthread_exit(0);
}

int main(int argc, char ** argv){
	//pid_t cpid;
	//scanf("%s", string);
	//printf("%s\n", string);
	//buf = string[0];
	int count = 0;
	char * buf;
	scanf("%c", buf);
	printf("buf: %c\n", *buf);
	pthread_t pid1, pid2, pid3;
	int val = pipe(pipeEnd);
	if(val == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	while(*buf != 10){
		close(pipeEnd[0]);
		pthread_create(&pid1, NULL, shift_lower_case, NULL);
		printf("BEFOREWRITE\n");
		//write(pipeEnd[1], buf, 1);
		//write(STDIN_FILENO, buf, 1);
		close(pipeEnd[1]);
		printf("\nAFTERWRITE\n");
		wait(NULL);
		//pthread_create(&pid3, NULL, &check_overflow, &buf);
		pthread_join(pid1, NULL);
		pthread_create(&pid2, NULL, shift_5_right, NULL);
		pthread_join(pid2, NULL);
		printf("count: %d\n", count);
		scanf("%c", buf);
		count++;
		*buf = 10;
	}
	/*
	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	cpid = fork();
	if (cpid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (cpid == 0) {    // Child reads from pipe 
		close(pipefd[1]);          // Close unused write end 
		while (read(pipefd[0], &buf, 1) > 0)
			write(STDOUT_FILENO, &buf, 1);
			
		write(STDOUT_FILENO, "\n", 1);
		close(pipefd[0]);
		_exit(EXIT_SUCCESS);
	}
	else{            // Parent writes argv[1] to pipe 
		close(pipefd[0]);          // Close unused read end 
		write(pipefd[1], argv[1], strlen(argv[1]));
		close(pipefd[1]);          // Reader will see EOF 
		wait(NULL);                // Wait for child 
		exit(EXIT_SUCCESS);
	}*/
	return EXIT_SUCCESS;
}
