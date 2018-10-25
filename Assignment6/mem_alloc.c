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
  struct memBlocks * next;
};

int main(int argc, char ** argv){
  if(argc != 4 && argc != 5){
    printf("Failure\n");
    return EXIT_FAILURE;
  }
  int memT = -1;
  char alg;
  int i;
  for(i = 1; i < 5; i++){
    printf("argv[i]: %s\n", argv[i]);
    if(argv[i] == "-s"){
      printf("mem\n");
      memT = atoi(argv[i+1]);
    }
    else if(argv[i] == "-f"){
      printf("-f\n");
      alg = 'f';
    }
    else if(argv[i] == "-b"){
      printf("-b\n");
      alg = 'b';
    }
    else if(argv[i] == "-w"){
      printf("-w\n");
      alg = 'w';
    }
  }
  printf("memT: %d, alg: -%c\n",memT, alg);
  return EXIT_SUCCESS;
}
