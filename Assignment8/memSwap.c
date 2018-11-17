#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int pageFaults = 0, swapSaves = 0, swapLoads = 0, numFramesInUse = 0;
char * algorithm;
int numFrames;
int ** swapSpace;

typedef int bool;
#define true 1
#define false 0

void printT(int ** table){
  int i;
  printf("\tLIST<\n");
  for(i = 0; i < numFrames; i++){
    printf("page:\t\t %d\n", table[i][0]);
    printf("offset:\t\t %d\n", table[i][1]);
    printf("valid / invalid: %c\n", table[i][2]);
    printf("age:\t\t %d\n", table[i][3]);
    printf("clean / dirty:   %c\n", table[i][4]);
    if(i < numFrames-1){
      printf("\n");
    }
  }
  printf("\t>LIST\n");
}

void init(int ** table){
  int i, j;
  swapSpace = malloc(numFrames*sizeof(unsigned int *));
  for(i = 0; i < numFrames; i++){
    swapSpace[i] = malloc(sizeof(int));
    for(j = 0; j < 5; j++){
      if(j == 2){
        table[i][j] = 'i';
      }
      else if(j == 3){
        table[i][j] = 0;
      }
      else if(j == 4){
        table[i][j] = 'c';
      }
      else{
        table[i][j] = -1;
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

void age(int ** table){
  int i;
  for(i = 0; i < numFrames; i++){
    if(table[i][2] == 'v'){
      table[i][3]++;
    }
  }
  printT(table);
}

bool exists(int ** table, long page){
  int i;
  for(i = 0; i < numFrames; i++){
    if(table[i][0] == page){
      printf("true\n");
      return true;
    }
  }
  printf("false\n");
  return false;
}

void setTable(int ** table, int index, long page, long offset, int stat, int count, int cORd){
  table[index][0] = page;
  table[index][1] = offset;
  table[index][2] = stat;
  table[index][3] = count;
  table[index][4] = cORd;
}

void fifo(unsigned int * pt, int ** table, long offset, long page, char action){
  printf("FIFO\n");
    int i, j, k;
  // not all frames are in use
  if(!exists(table, page)){
    if(numFramesInUse < numFrames){
      pt[page] = offset;
      pageFaults++;
      k = 'c';
      if(action == 'w'){
        k = 'd';
      }
      setTable(table, numFramesInUse, page, offset, 'v', 0, k);
      numFramesInUse++;
    }
    else{
      k = table[0][3];
      j = 0;
      for(i = 1; i < numFrames; i++){
        printf("k: %d VS table[%d][3]: %d\n", k, i, table[i][3]);
        if(table[i][3] > k){
          k = table[i][3];
          j = i;
        }
      }
      if(table[j][4] == 'd'){

      }
      pt[page] = offset;
      pageFaults++;
      k = 'c';
      if(action == 'w'){
        k = 'd';
      }
      setTable(table, j, page, offset, 'v', 0, k);
    }
    age(table);
  }
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
    int ** physicalTable; // in/valid checker, LRU/FIFO data
    pageTable = malloc(pow(2,20)*sizeof(unsigned int));
    physicalTable = malloc(numFrames*sizeof(int *));
    for(i = 0; i < numFrames; i++){
      physicalTable[i] = malloc(sizeof(int));
    }
    init(physicalTable);
    scan = scanf("%c%ld\n", &action, &logical);
    while(scan == 2){
      physical = logical & (mask - 1);
      temp = logical;
      logical = logical>>12;
      printf("scan: %d, action: %c, logical: %ld, page: %ld, physical: %ld\n", scan, action, temp, logical, physical);
      if(algorithm[0] == 'f'){
        fifo(pageTable, physicalTable, physical, logical, action);
      }
      else{
        printf("LRU\n");
      }
      scan = scanf("%c %ld\n", &action, &logical);
    }
    printT(physicalTable);
    free(pageTable);
    for(i = 0; i < numFrames; i++){
        free(physicalTable[i]);
        free(swapSpace[i]);
      }
    free(physicalTable);
    free(swapSpace);
    printf("Summary\n");
    printf("Total page faults: %d\n", pageFaults);
    return EXIT_SUCCESS;
}
