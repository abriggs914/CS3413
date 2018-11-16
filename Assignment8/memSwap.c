#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int pageFaults, swapSaves, swapLoads;
char * algorithm;
int numFrames;

typedef int bool;
#define true 1
#define false 0

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
    }
    return true;
}

// char * read(char * line){
//
// }

int main(int argc, char **argv){
    if(argc < 3 || argc > 4){ // early fail condition
        printf("Usage: ./memSwap # -(one of; -f, -l) < fileName.txt (optional)\n");
        return EXIT_FAILURE;
    }
    if(!decipher(argv) || numFrames < 0 || numFrames > pow(2,32)){
      printf("Adjust numFrames or algorithm.\nUsage: ./memSwap # -(one of; -f, -l) < fileName.txt (optional)\n");
      return EXIT_FAILURE;
    }
    printf("algorithm: %s, numFrames: %d\n", algorithm, numFrames);

    int scan, mask = pow(2,12);
    char action;
    long logical = 0, physical;
    unsigned int * pageTable;
    pageTable = malloc(pow(2,20)*sizeof(unsigned int));
    scan = scanf("%c%ld\n", &action, &logical);
    while(scan == 2){
      printf("scan: %d, action: %c, logical: %ld, physical: %ld\n", scan, action, logical, ((logical>>12)));
      scan = scanf("%c %ld\n", &action, &logical);
    }
    free(pageTable);
    return EXIT_SUCCESS;
}
