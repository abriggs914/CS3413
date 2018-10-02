#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//char * shift_lower_case(void * line){
char * shift_lower_case(char * line){
	//char * ptr = (char *)line;
	char * ptr = line;
	int i;
	for(i = 0; ptr != NULL && i < 150; i++){
		if(*ptr > 64 && *ptr < 91){
			*ptr = *ptr +32;
			printf("i: %d, %c\n", i, *ptr);
		}
		ptr++;
	} 
	return ptr;
}

char * shift_5_right(char * line){
	return line;
}

char * check_overflow(char * line){
	return line;
}

int main(int argc, char ** argv){
	char string[50];
	scanf("%s", string);
	printf("String %s\n", string);
	int count = 0, i;
	char * ptr = string;
	printf("shift_lower_case(%s): %s\n", ptr, shift_lower_case(ptr));
	/*int pipefd[2];
	pid_t cpid;
	char buf;
	char string[50];
	//scanf("%s", string);
	//printf("%s\n", string);
	//buf = string[0];
	int count = 0, i;
	buf = scanf("%c", &buf);
	pthread_t pid[3];
	while(buf != 10){
		pthread_create(&pid[0], NULL, &shift_lower_case, &buf);
		pthread_create(&pid[1], NULL, &shift_5_right, &buf);
		pthread_create(&pid[2], NULL, &check_overflow, &buf);
		pthread_join(*pid, NULL);
		buf = scanf("%c", &buf);
		string[count] = buf;
		count++;
	}
	printf("count: %d\n", count);
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <string>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	cpid = fork();
	if (cpid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (cpid == 0) {    /* Child reads from pipe */
	//	close(pipefd[1]);          /* Close unused write end */
	//	while (read(pipefd[0], &buf, 1) > 0)
	//		write(STDOUT_FILENO, &buf, 1);
			
	//	write(STDOUT_FILENO, "\n", 1);
	//	close(pipefd[0]);
	//	_exit(EXIT_SUCCESS);
	//}
	//else{            /* Parent writes argv[1] to pipe */
	//	close(pipefd[0]);          /* Close unused read end */
	//write(pipefd[1], argv[1], strlen(argv[1]));
	//	close(pipefd[1]);          /* Reader will see EOF */
	//	wait(NULL);                /* Wait for child */
	//	exit(EXIT_SUCCESS);
	//}*/
}
