/*
* CS3413 Assignment 7
* Nov 2018
* Avery Briggs
* 3471065
*
* C program to read logical memory address for a
* 2^32 virtual space. Program converts to a physical
* address, assuming that the page sizing is 2^12 (4 kb)
* A mamximum number of frames that may be loaded into
* memory at one time is given from the command line.
* Usage: ./addressConvert # fileName.txt (optional)
* When entering logical addresses on the command line
* the reporting will appear to be delayed, but the
* principal form for input should be redirection on
* the command line to a prepared text file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

// offsetMask - used to calculate the offset of a logical address
// pageMask - the size of a page
// pageFaults - running total of pagefaults
const long int offsetMask = (pow(2,12)-1), pageMask = pow(2,12);
int pageFaults = 0;

// Function returns true or false whether an address's
// page is already loaded.
bool check(int table[], int page, int size){
  for(int i = 0; i < size; i++){
    if(table[i] == page){
      return true;
    }
  }
  return false;
}

//Function is called to insert a page into memory
void add(int page, int table[], int size){
  if(check(table, page, size)){
    return;
  }
  for(int i = 0; i < size; i++){
    if(table[i] < 0){
      table[i] = page;
      pageFaults++;
      return;
    }
  }
}

// Funtion uses the FIFO algorithm to simulate
// paging. It shuffles the pagetable array to make
// room at the end
void fifo(int table[], int size){
  if(size == 1){
    table[0] = -1;
  }
  else{
    int i,j;
    for(i = 0, j = 1; j < size; i++, j++){
      table[i] = table[j];
    }
    table[i] = -1;
  }
}

int main(int argc, char ** argv){
  if(argc < 2 || argc > 3 || atoi(argv[1]) < 0 || atoi(argv[1]) > pow(2,20)){
    printf("Usage: ./addressConvert # FILENAME.txt (optional)\n");
    return EXIT_FAILURE;
  }
  long int num;
  char numS[32];
  int i, numFrames = atoi(argv[1]), y;
  int numFramesInUse = 0, phPage, phOff;
  int * pageTable;
  pageTable = malloc(numFrames*sizeof(int));
  for(i = 0; i < numFrames; i++){
    pageTable[i] = -1;
  }
  y = scanf("%ld\n", &num);
  printf("Logical\t\t\tPhysical\n\t\t\tPage\tOffset\n");
  while(y == 1){
    if(num > pow(2,32)){
      printf("Number is too large. only 2^32 memory\n");
    }
    else if(num < 0){
      printf("Number is too small. only positives\n");
    }
    else{
      sprintf(numS, "%ld", num);
      phPage = num/pageMask;
      phOff = num & offsetMask;
      printf("%ld\t%s->\t%d\t%d\n", num, ((strlen(numS)<8)?"\t":""), phPage, phOff);
      if(!check(pageTable, phPage, numFrames) && numFramesInUse < numFrames){
          add(phPage, pageTable, numFrames);
          numFramesInUse++;
      }
      else if(numFramesInUse == numFrames){
        if(!check(pageTable, phPage, numFrames)){
          fifo(pageTable, numFrames);
          numFramesInUse--;
          add(phPage, pageTable, numFrames);
          numFramesInUse++;
        }
      }
    }
    y = scanf("%ld\n", &num);
  }
  free(pageTable);
  printf("Total page faults: %d\n", pageFaults);
}
