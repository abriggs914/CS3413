/*
* CS3413 Assignment 6
* Oct.31/18
* Avery Briggs
* 3471065
*
* C program to simulate memory
* allocations for processes.
* The program uses one of three
* algorithms for allocating memory.
* First fit, Best fit, Worst fit.
* command line format:
* ./mem_alloc -s # [-f,-b,-w] (< file.txt)
* *one of the three algorithms.
*/
// linked list for process holes and processes allocs. needs multiple points

#include <stdio.h>
#include <stdlib.h>
//#include <>

struct memBlocks{
  int start;
  int end;
  int inUse;
  struct memBlocks * next;
};

struct memBlocks * base;

void enQueue(struct memBlocks * newBlock){
	if(base == NULL && rear == NULL){ // queue is empty
		front = rear = newBlock;
    strcpy(front->driver, carIn->driver);
    strcpy(rear->driver, carIn->driver);
    base->next = NULL;
		return;
	}
	rear->next = newBlock;
  strcpy(rear->next->driver, carIn->driver);
	rear = newBlock;
  strcpy(rear->driver, carIn->driver);
  rear->next = NULL;
	return;
}

int main(int argc, char ** argv){
  if(argc != 4 && argc != 5){
    printf("Failure\n");
    return EXIT_FAILURE;
  }
  int memT = atoi(argv[2]);
  char alg = argv[3][1];
  int i;
  printf("memT: %d, alg: -%c\n",memT, alg);
  return EXIT_SUCCESS;
}
