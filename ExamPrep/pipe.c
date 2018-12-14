#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int main (int argc, char ** argv){
  if(argc > 2){
    printf("Use an input file\n");
    return EXIT_FAILURE;
  }
  int scan;
  char letterIn;
  pthread_t pid[3];
  scan = scanf("%c",letterIn);
  while(scan == 1){
    pthread_create(&pid[0], &NULL, toLower(), &NULL);
  }
}
