/*
* CS3413 Assignment 5
* Oct 2018
* Avery Briggs
* 3471065
*
*
* C program to solve a bridge queueing problem.
* Program reads input from stdIn, and requires a
* command line argument integer to act tell the
* program how long it takes a car to cross the bridge
* Program uses pthreads to simulate cars waiting
* for a chance to cross. Once 3 consecutive cars
* have crossed from the same side, the flow of traffic
* automatically changes to prevent long wait times.
* Program output is a list of the traffic summary,
* including driver's names and what direction they
* were heading.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

/*
* Car struct to store a driver's data.
* driver - name
* direction - N or S
* arrival - when the got to the bridge
* next - points to the next car that arrived
*/
struct car{
  char * driver;
  int direction;
  int arrival;
  struct car * next;
};

int speedLimit = 0;
int carsOnBridge = 0; // active car counter
int carsCrossedInOneDirection = 0; //counts cars going the same way
int currDir = -1;
int flow = -1; //flow of traffic, 'S'(83)-Southbound, 'N'(78)-NorthBound
struct car * front; // fornt of the car list
struct car * rear; //end of the car list.

pthread_mutex_t mutexB = PTHREAD_MUTEX_INITIALIZER;

//Function prints the data of a car.
void printCar(struct car * carIn){
  printf("CAR\n\tcarIn->driver:\t\t%s\n\tcarIn->direction:\t%c\n\tcarIn->arrival:\t\t%d\n\n",carIn->driver,carIn->direction,carIn->arrival);
}

// Function calculates and returns the length of a given list
int length(struct car * headIn){
    int num = 0;
    struct car * current;
    for(current = headIn; current != NULL; current = current->next){
      num++;
    }
    return num;
}

//Function is called as a helper to ArriveBridge
//prints the Summary table, and adjusts bridge status data
//Funnction also kills car thread
void * ExitBridge(void * carIn){
  struct car * temp = (struct car *)carIn;
  pthread_mutex_lock(&mutexB);
  carsCrossedInOneDirection++;
  char * s = ((temp->direction == 'S')? "South" : "North");
  if(temp->direction != currDir){
    printf("Direction: %s\n", s);
    currDir = temp->direction;
  }
  printf("%s\n", temp->driver);
  carsOnBridge--;
  pthread_mutex_unlock(&mutexB);
  pthread_exit(0);
}

//Function is called by the pthread car to decide
//when the car is allowed to cross the bridge, and
//how many at a time. Uses sleep to simulate the
//crossing of a car at the posted speedLimit.
void * ArriveBridge(void * carIn){
  struct car * temp = (struct car *) carIn;
  int p = -1, dir = temp->direction, j = 0;
  while(p != dir){
    sleep(1);
    pthread_mutex_lock(&mutexB);
    if(carsCrossedInOneDirection == 3){
      carsCrossedInOneDirection = 0;
      flow = ((flow == 'N')? 'S' :  'N');
    }
    if(flow == -1 || (carsOnBridge == 0 && j > speedLimit)){ // bridge empty, set direction
      flow = dir;
      carsCrossedInOneDirection = 0;
    }
    p = flow;
    if(carsCrossedInOneDirection < 3 && dir == flow && carsOnBridge < 3){ // room for car to cross
      carsOnBridge++;
      sleep(speedLimit);
      j = 0;
      break;
    }
    j++; //idle counter, <2 people crossing and <3 cars waiting
    if(j >= speedLimit){
      flow = -1;
      carsCrossedInOneDirection = 0;
      j = 0;
    }
    pthread_mutex_unlock(&mutexB);
    //printf("temp->driver: %s is still waiting\n", temp->driver);
  }
  pthread_mutex_unlock(&mutexB);
  ExitBridge(carIn);
  return NULL;
}

//Function queues all the drivers when given on input.
void enQueue(struct car * carIn) {
	if(front == NULL && rear == NULL){ // queue is empty
		front = rear = carIn;
    strcpy(front->driver, carIn->driver);
    strcpy(rear->driver, carIn->driver);
    front->next = NULL;
		return;
	}
	rear->next = carIn;
  strcpy(rear->next->driver, carIn->driver);
	rear = carIn;
  strcpy(rear->driver, carIn->driver);
  rear->next = NULL;
	return;
}

//Function returns a car at a given
//index from the list of cars.
struct car * element(int i){
  //printf("element [%d]\n",i);
  struct car * temp = front;
  int j = 0;
  while(temp != rear && j != i){
    temp = temp->next;
    j++;
  }
  //printCar(temp);
  return temp;
}

int main(int argc, char **argv){
  char driverIn[20];
  char title[100];
  int directionIn, arrivalIn, p, t, i;
  struct car * carIn;
  if(argc != 2){ // invalid input exit condition
    printf("Usage: a.out n (n is an integer)\n");
    return EXIT_FAILURE;
  }
  speedLimit = atoi(*(argv+1));
  scanf("%[^\n]", title);
  p = scanf("%s %lc %d", driverIn, &directionIn, &arrivalIn);
  while(p == 3 && driverIn != NULL){ // read in and list all cars
    carIn = malloc(sizeof(struct car *));
    carIn->driver = malloc(20*sizeof(char));
    strcpy(carIn->driver, driverIn);
    carIn->direction = directionIn;
    carIn->arrival = arrivalIn;
    enQueue(carIn);
    p = scanf("%s %lc %d", driverIn, &directionIn, &arrivalIn);
  }
  i = 0;
  p = length(front);
  pthread_t cars[p];
  carIn = element(i);
  for(t = 0; carIn != NULL && t < 100; t++){ // clock is ticking (simulation)
    if(carIn->arrival == t){
      //printf("ARRIVAL: %s @ t = %d\n",carIn->driver, t);
      pthread_create(&cars[i], NULL, ArriveBridge, carIn);
      i++;
    }
    carIn = element(i);
    if(i == p){
      carIn = NULL;
    }
    //printf("t: %d\n", t); // time display
    sleep(1);
  }
  for(i = 0; i < p; i++){
    pthread_join(cars[i], NULL);
    free(element(i)->driver);
    free(element(i));
  }
/*  for(i = 0; i < p; i++){
    free(element(p)->driver);
    free(element(p));
  }*/
  return EXIT_SUCCESS;
}
