/*
* CS3413
* Lab 3
* Avery Briggs
* 3471065
*
* C program to solve a smokers
* problem using threads and
* semaphores
*/

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>


#define matches 'm'
#define paper 'p'
#define tabacco 't'
sem_t semaphore;
pthread_t smokers[3];
pthread_t agent;
int cigarettesMade = 0;

void * checkTable(void * arg){
  time_t tm;
  srand((unsigned) time(&tm));
  sleep(1);
  int n = rand() % 122;
  //int a = sem_wait(&semaphore);

  printf("dying @ t = %d\n", n);
  pthread_exit(0);
}

void * deal(void * arg){
  int a, first, second;
  time_t tm;
  srand((unsigned) time(&tm));
  while(cigarettesMade < 10){
    a = sem_wait(&semaphore);
    while(first != tabacco && first != paper && first != matches){
      first = rand() % 122;
    }
      sleep(1);
    //printf("first: %d\n", first);
    while(second != first && second != paper && second != tabacco && second != matches){
      second = rand() % 122;
    }
    sleep(1);
    //printf("second: %d\n", second);
    char *f;
    char *s;
    if(first == tabacco){
      f = "tabacco";
    }
    else if(first == paper){
      f = "paper";
    }
    else{
      f = "matches";
    }
    if(second == tabacco){
      s = "tabacco";
    }
    else if(second == paper){
      s = "paper";
    }
    else{
      s = "matches";
    }
    printf("first: %d, second: %d\n", first, second);
    printf("first: %s, second: %s\n\n", f, s);
    first = second = -1;
    cigarettesMade++;
    a = sem_post(&semaphore);
  }
  return NULL;
}

int main(int argc, char ** argv){
  int a = 10, i;
  pthread_create(&agent, NULL, deal, NULL);
  for (i = 0; i < 3; i++){
    pthread_create(&smokers[i], NULL, checkTable, NULL);
  }
  a = sem_init(&semaphore, 0, 3);
  printf("a: %d\n",a);
  a = sem_post(&semaphore);
  printf("a: %d\n",a);
  printf("a: %d\n",a);
  pthread_join(agent, NULL);
  for (i = 0; i < 3; i++){
    pthread_join(smokers[i], NULL);
  }
  return EXIT_SUCCESS;
}
