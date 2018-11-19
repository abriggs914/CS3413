/*
* CS3413 Assignment 8
* Nov 2018
* Avery Briggs
* 371065
*
* C program to convert logical memory addresses for
* a virtual space of 2^32 bits, to physical addresses.
* The number of available frames is passed through
* the command line (max 2^20, min 0), along with a
* choice of algorithm (FirstInFirstOut or
* LeastRecentlyUsed). Program reports the number of
* encountered page faults, as well as a summary of
* swaps to and from the disk swap space.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

unsigned int pageBits; // set to 2^20
// running totals
int pageFaults = 0, swapSaves = 0, swapLoads = 0, numFramesInUse = 0;
char * algorithm; // user selected algorithm
int numFrames; // user selected number of frames
int ** swapSpace; // array simulates swap space

typedef int bool;
#define true 1
#define false 0

// Function prints list of occupied frames
void printT(int ** table){
  int i;
  printf("\t\tLIST<\n");
  for(i = 0; i < numFrames; i++){
    if(table[i][0] > -1){
      printf("i: %d\n", i);
      printf("\tpage:\t\t %d\n", table[i][0]);
      printf("\toffset:\t\t %d\n", table[i][1]);
      printf("\tvalid / invalid: %c\n", table[i][2]);
      printf("\tage:\t\t %d\n", table[i][3]);
      printf("\tclean / dirty:   %c\n", table[i][4]);
      printf("\taction:\t\t %c\n", ((table[i][5]==0)?'-':table[i][5]));
      printf("\tlogical:\t %ld\n", (long)((4096*table[i][0])+table[i][1]));
      printf("\tphysical:\t %ld\n", (long)((table[i][0]<numFrames)?((4096*(table[i][0]+1))+table[i][1]):table[i][0]));
    }
    if(i < numFrames-1){
      printf("\n");
    }
  }
  printf("\t\t>LIST\n");
}

// function intializes swapSpace, and helper frame table
// using heap
void init(int ** table){
  int i, j;
  swapSpace = malloc(pageBits*sizeof(unsigned int *));
  for(i = 0; i < pageBits; i++){
    swapSpace[i] = malloc(sizeof(int));
  }
  for(i = 0; i < numFrames; i++){
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

// Function ages all the frames in use.
void age(int ** table){
  int i;
  for(i = 0; i < numFrames; i++){
    if(table[i][2] == 'v'){
      table[i][3]++;
    }
  }
  // printf("numFramesInUse of numFrames:\n\t%d\tVS\t%d\n", numFramesInUse, numFrames);
  // printT(table);
}

// Function checks whether a page is already framed
bool exists(int ** table, long page){
  int i;
  for(i = 0; i < numFrames; i++){
    if(table[i][0] == page){
      return true;
    }
  }
  return false;
}

// Function checks whether a page is in the swapSpace
bool isInSwapSpace(long page, long offset, char action){
  if(swapSpace[page][1] == 'v'){
    return true;
  }
  return false;
}

 // Function sets all attributes of a framed page
 // to the physicalTable array.
 // I use the array as kind of a struct
void setTable(int ** table, int index, long page, long offset, int stat, int count, int cORd, char action){
  table[index][0] = page;
  table[index][1] = offset;
  table[index][2] = stat;
  table[index][3] = count;
  table[index][4] = cORd;
  table[index][5] = action;
}

// Function sets a framed page to update the pageTable
void setPageTable(int ** pt, long page, long offset){
  if(pt[page][1] == 'i'){ // page table entry is empty
    pt[page][0] = offset;
    pt[page][1] = 'v';
    return;
  }
  pt[page][0] = offset;
}

// Function saves a page to the swapSpace array.
void swapSpaceEntry(int ** pt, int ** table, int index){
  int page = table[index][0];
  int offset = table[index][1];
  if(isInSwapSpace(page, offset, 'w')){
    swapSpace[page][0] = offset;
  }
  else{
    swapSpace[page][0] = offset;
    swapSpace[page][1] = 'v';
  }
  swapSaves++;
}

// Function uses FIFO algorithm to assign frames to
// incoming pages. Uses a helper array (physicalTable)
// to keep track of page ages and r or w status.
void fifo(int ** pt, int ** table, long offset, long page, char action){
  int i, j, k;
  // not all frames are in use
  if(!exists(table, page)){
    if(numFramesInUse < numFrames){
      setPageTable(pt, page, offset);
      pageFaults++;
      k = 'c';
      if(action == 'w'){
        k = 'd';
      }
      setTable(table, numFramesInUse, page, offset, 'v', 0, k, action);
      numFramesInUse++;
    }
    else{
      k = table[0][3];
      j = 0;
      for(i = 1; i < numFrames; i++){
        if(table[i][3] > k){
          k = table[i][3];
          j = i;
        }
      }
      if(table[j][4] == 'd'){
        swapSpaceEntry(pt, table, j);
      }
      setPageTable(pt, page, offset);
      if(!isInSwapSpace(page, offset, action)){
        pageFaults++;
      }
      else{
        swapLoads++;
      }
      k = 'c';
      if(action == 'w'){
        k = 'd';
      }
      setTable(table, j, page, offset, 'v', 0, k, action);
    }
    age(table);
  }
}

// Function uses LRU algorithm to assign frames to
// incoming pages. Uses a helper array (physicalTable)
// to keep track of page ages and r or w status.
void lru(int ** pt, int ** table, long offset, long page, char action){
    int i, j, k;
  // not all frames are in use
  if(!exists(table, page)){
    if(numFramesInUse < numFrames){
      setPageTable(pt, page, offset);
      pageFaults++;
      k = 'c';
      if(action == 'w'){
        k = 'd';
      }
      setTable(table, numFramesInUse, page, offset, 'v', 0, k, action);
      numFramesInUse++;
    }
    else{
      k = table[0][3];
      j = 0;
      for(i = 1; i < numFrames; i++){
        // printf("k: %d VS table[%d][3]: %d, table[%d][0]: %d\n", k, i, table[i][3], i, table[i][0]);
        if(table[i][3] > k){
          k = table[i][3];
          j = i;
        }
      }
      if(table[j][4] == 'd'){
        swapSpaceEntry(pt, table, j);
      }
      setPageTable(pt, page, offset);
      if(!isInSwapSpace(page, offset, action)){
        pageFaults++;
      }
      else{
        swapLoads++;
      }
      k = 'c';
      if(action == 'w'){
        k = 'd';
      }
      setTable(table, j, page, offset, 'v', 0, k, action);
    }
    age(table);
  }
}

int main(int argc, char **argv){
    // early fail condition
    pageBits = pow(2,20);
    if(argc < 3 || argc > 4){
        printf("Usage: ./memSwap # -(one of; -f, -l) < fileName.txt (optional)\n");
        return EXIT_FAILURE;
    }
    // logic check fail condition
    if(!decipher(argv) || numFrames < 1 || numFrames > pageBits){
      printf("Adjust numFrames or algorithm.\nUsage: ./memSwap # -(one of; -f, -l) < fileName.txt (optional)\n");
      return EXIT_FAILURE;
    }
    printf("algorithm: %s, numFrames: %d\n", algorithm, numFrames);

    int scan, i, mask = pow(2,12);
    char action;
    long logical, physical, temp;
    unsigned int val;
    int ** physicalTable, ** pageTable; // in/valid checker, LRU/FIFO data, pageTable with in/valid data
    pageTable = malloc(pageBits*sizeof(unsigned int *));
    physicalTable = malloc(numFrames*sizeof(int *));
    for(i = 0; i < pageBits; i++){
      pageTable[i] = malloc(sizeof(unsigned int));
    }
    for(i = 0; i < numFrames; i++){
      physicalTable[i] = malloc(sizeof(int));
    }
    init(physicalTable);
    scan = scanf("%c %ld\n", &action, &logical);
    printf("Logical\t\tPhysical\n");
    while(scan == 2){
      physical = logical & (mask-1);
      temp = logical;
      logical = logical>>12;
      if(logical < numFrames-1){
        val = (4096*(logical+1))+physical;
      }
      else{
        val = physical;
      }
      // printf("scan: %d, action: %c, logical: %ld, page: %ld, physical: %ld\n", scan, action, temp, logical, physical);
      printf("%u\t->\t%u\n", (unsigned int)temp, val);
      if(algorithm[0] == 'f'){
        fifo(pageTable, physicalTable, physical, logical, action);
      }
      else{
        lru(pageTable, physicalTable, physical, logical, action);
      }
      scan = scanf("%c %ld\n", &action, &logical);
    }
    // printT(physicalTable);
    for(i = 0; i < pageBits; i++){
      free(pageTable[i]);
      free(swapSpace[i]);
    }
    free(pageTable);
    for(i = 0; i < numFrames; i++){
        free(physicalTable[i]);
    }
    free(physicalTable);
    free(swapSpace);
    printf("Summary\n");
    printf("Total page faults: %d\n", pageFaults);
    printf("Total swap space saves: %d\n", swapSaves);
    printf("Total swap space loads: %d\n", swapLoads);
    return EXIT_SUCCESS;
}
