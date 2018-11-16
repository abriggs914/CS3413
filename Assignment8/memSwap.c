#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int pageFaults = 0, swapSaves = 0, swapLoads = 0;
char * algorithm;
int numFrames;

typedef int bool;
#define true 1
#define false 0

void init(int *** table){
  int i, j, k;
  for(i = 0; i < numFrames; i++){
    for(j = 0; j < 1; j++){
      table[i][j] = 'i';
      for(i = 0; i < numFrames; i++){
        table[i][j][k] = 0;
      }
    }
  }
}

//Function is called to pull the algorithm and numFrames values from argv
bool decipher(char ** line){
    int i;
    for(i = 1; i < 3; i++){
        if(strlen(line[i]) == 2){
          if(line[i][0] == '-'){
            if(line[i][1] == 'f'){
                algorithm = "first in first out";
            }
            else if(line[i][1] == 'l'){
                algorithm = "least recently used";
            }
            else{
                return false;
            }
          }
          else{
              numFrames = atoi(line[i]);
          }
        }
        else{
            numFrames = atoi(line[i]);
        }
    }
    return true;
}

// void fifo(unsigned int arr[], )

int main(int argc, char **argv){
    // early fail condition
    if(argc < 3 || argc > 4){
        printf("Usage: ./memSwap # -(one of; -f, -l) < fileName.txt (optional)\n");
        return EXIT_FAILURE;
    }
    // logic check fail condition
    if(!decipher(argv) || numFrames < 1 || numFrames > pow(2,20)){
      printf("Adjust numFrames or algorithm.\nUsage: ./memSwap # -(one of; -f, -l) < fileName.txt (optional)\n");
      return EXIT_FAILURE;
    }
    printf("algorithm: %s, numFrames: %d\n", algorithm, numFrames);

    int scan, i, j, mask = pow(2,12);
    char action;
    long logical, physical, temp;
    unsigned int * pageTable;
    int *** physicalTable; // in/valid checker, LRU/FIFO data
    pageTable = malloc(pow(2,20)*sizeof(unsigned int));
    physicalTable = malloc(numFrames*sizeof(unsigned int));
    for(i = 0; i < numFrames; i++){
      physicalTable[i] = malloc(sizeof(char));
      for(j = 0; j < 1; j++){
        physicalTable[i][j] = malloc(sizeof(unsigned int));
      }
    }
    scan = scanf("%c%ld\n", &action, &logical);
    while(scan == 2){
      physical = logical & (mask - 1);
      temp = logical;
      logical = logical>>12;
      printf("scan: %d, action: %c, logical: %ld, page: %ld, physical: %ld\n", scan, action, temp, logical, physical);
      scan = scanf("%c %ld\n", &action, &logical);
    }
    int k;
    init(physicalTable);
    for(i = 0; i < numFrames; i++){
      for(j = 0; j < 1; j++){
        for(k = 0; k < 1; k++){
          printf("physicalTable[%d][%d][%d]: %d\n", i, j, k, physicalTable[i][j][k]);
        }
      }
    }
    free(pageTable);
    for(i = 0; i < numFrames; i++){
      for(j = 0; j < 1; j++){
        free(physicalTable[i][j]);
      }
      free(physicalTable[i]);
    }
    free(physicalTable);
    return EXIT_SUCCESS;
}
