#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>

#define SIZE 15

int main()
{
    int to_child_fd[2];
    int to_parent_fd[2];
    pipe(to_child_fd);
    pipe(to_parent_fd);

    if (fork() == 0) {
    	char message[SIZE];
    	int length;

   		 close(to_child_fd[1]);  /* child closes write side of child  pipe */
   		 close(to_parent_fd[0]); /* child closes read  side of parent pipe */
   		 length = read(to_child_fd[0], message, SIZE);
    	for (int i = 0; i < length; i++) {
       		 message[i] = toupper(message[i]);
    	}
   		 printf("child: %s\n", message);
   		 write(to_parent_fd[1], message, strlen(message) + 1);
    	 close(to_parent_fd[1]);
    } 
	else{
		char phrase[SIZE];
		char message[SIZE];
   		printf("please type a sentence : ");
    	scanf("%s", phrase);
    	close(to_parent_fd[1]); /* parent closes write side of parent pipe */
   		close(to_child_fd[0]);  /* parent closes read  side of child  pipe */
    	write(to_child_fd[1], phrase, strlen(phrase) + 1);
    	close(to_child_fd[1]);
   		read(to_parent_fd[0], message, SIZE);
   		printf("the original message: %s\nthe capitalized version: %s\n", phrase, message);
   	}
    return 0;
}
