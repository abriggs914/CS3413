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
int stopCond = 200; //how many cigarettes would you like to make?
int sleepTime = 20; //how long does it take to smoke a cigarette?
int firstItem;
int secondItem;

char * itemName(int n){
  if(n == tabacco){
    return "tabacco";
  }
  else if(n == paper){
    return "paper";
  }
  else{
    return "matches";
  }
}

void * checkTable(void * arg){
  time_t tm;
  srand((unsigned) time(&tm));
  int * myItem = (int *)arg;
  int a, m;
  while(cigarettesMade < stopCond){
    switch(*myItem){
      case tabacco :  if(firstItem == paper || firstItem == matches){
                        if(secondItem == paper || secondItem == matches){
                          a = sem_wait(&semaphore);
                          firstItem = secondItem = -1;
                          cigarettesMade++;
                          m = (rand()%sleepTime);
                          printf("cigarette made by tabacco, smoking for: %d\n", m);
                          a = sem_post(&semaphore);
                          sleep(m);
                        }
                      }
                      break;
      case paper :  if(firstItem == tabacco || firstItem == matches){
                        if(secondItem == tabacco || secondItem == matches){
                          a = sem_wait(&semaphore);
                          firstItem = secondItem = -1;
                          cigarettesMade++;
                          m = (rand()%sleepTime);
                          printf("cigarette made by paper, smoking for: %d\n", m);
                          a = sem_post(&semaphore);
                          sleep(m);
                        }
                      }
                      break;
      case matches :  if(firstItem == paper || firstItem == tabacco){
                        if(secondItem == paper || secondItem == tabacco){
                          a = sem_wait(&semaphore);
                          firstItem = secondItem = -1;
                          cigarettesMade++;
                          m = (rand()%sleepTime);
                          printf("cigarette made by matches, smoking for: %d\n", m);
                          a = sem_post(&semaphore);
                          sleep(m);
                        }
                      }
                      break;
     default :  printf("something went wrong myItem: %d\n", *myItem);
                break;
    }
    //printf("%s sleep\n", itemName(*myItem));
    //sleep(1);
  }
  //printf("%s dying @ t = %d\n", itemName(*myItem), n);
  pthread_exit(0);
}

int dealFirst(){
  int first = rand() % 122;
  while(first != tabacco && first != paper && first != matches){
    first = rand() % 122;
  }
  return first;
}

int dealSecond(int first){
  time_t tm;
  srand((unsigned) time(&tm));
  int second = rand() % 122;
  while(second != paper && second != tabacco && second != matches){
    second = rand() % 122;
    while(second == first){
      second = rand() % 122;
    }
  }
  if(second == first){
    return dealSecond(first);
  }
  else{
    return second;
  }
}

void * deal(void * arg){
  int a, first, second;
  time_t tm;
  srand((unsigned) time(&tm));
  firstItem = secondItem = -1;
  while(cigarettesMade < stopCond){
    if(firstItem != -1 || secondItem != -1){
      //printf("dealer sleeps\n");
      sleep(1);
    }
    else{
      printf("cigarettesMade: %d\n\n", cigarettesMade);
      a = sem_wait(&semaphore);
      first = dealFirst();
      sleep(1);
      second = dealSecond(first);
      sleep(1);
      char *f = itemName(first);
      char *s = itemName(second);
      printf("Agent dealt : %s\tand\t%s\n", f, s);
      firstItem = first;
      secondItem = second;
      a = sem_post(&semaphore);
    }
  }
  printf("cigarettesMade: %d\n\n", cigarettesMade);
  pthread_exit(0);
  return NULL;
}

int main(int argc, char ** argv){
  int i, pa = 116, ta = 112, ma = 109;
  pthread_create(&agent, NULL, deal, NULL);
  pthread_create(&smokers[0], NULL, checkTable, &ta); //tabacco
  pthread_create(&smokers[1], NULL, checkTable, &pa); //paper
  pthread_create(&smokers[2], NULL, checkTable, &ma); //matches
  i = sem_init(&semaphore, 0, 3);
  pthread_join(agent, NULL);
  for (i = 0; i < 3; i++){
    pthread_join(smokers[i], NULL);
  }
  return EXIT_SUCCESS;
}
