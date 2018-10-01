#include <stdio.h>
#include <unistd.h>

int main()
{
  int pid,pid2;
  pid=fork();

  if (pid<0) {
    printf("fork failed");
    exit(-1);
  } else if (pid==0) {
    printf("child id is%d",getpid());
    execlp("/bin/ls","is",NULL);
    printf("\nsleeping for 2 seconds using pid of child class");
    sleep(2);

    printf("killing the child process");
    kill(getpid());
  } else {
    wait(NULL);
    printf("The parent id is %d\n",getpid());
    printf("The child id is %d\n",getpid());
    printf("\nsleeping for 3 seconds without pid");
    sleep(3);
    printf("\nchild completed\n");

    exit(0);
  }
}
