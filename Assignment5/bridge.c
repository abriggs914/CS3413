#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct car{
  char * driver;
  int direction;
  int arrival;
};

int main(int argc, char **argv){
  char * userIn;
  char title[100];
  int directionIn, arrivalIn, p;
  if(argc != 2){
    printf("Usage: a.out n (n is an integer)\n");
    return EXIT_FAILURE;
  }
  scanf("%[^\n]", title);
  printf("\ntitle: %s\n", title);
  p = scanf("%s %c %d", userIn, &directionIn, &arrivalIn);
  printf("\nuser: %s, direction: %d, arrival: %d\n", userIn, directionIn, arrivalIn);
  while(p == 3){
    printf("Car: %s,\tDirection: %c,\tArrival: %d\n", userIn, directionIn, arrivalIn);
    p = scanf("%s %c %d", userIn, &directionIn, &arrivalIn);
  }
  return EXIT_SUCCESS;
}
