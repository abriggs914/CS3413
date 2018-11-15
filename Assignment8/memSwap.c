#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int pageFaults, swapSaves, swapLoads;
char * algorithm;
int numFrames;

//Function is called to pull the memT and alg values from argv
void decipher(char ** line){
    int i;
    for(i = 1; i < 3; i++){
        printf("line[%d]: %s, strlen(line[1]: %d\n", i, line[1], strlen(line[i]));
        if(strlen(line[i]) == 2){
            if(line[i][1] == 'f' ){
                algorithm = "first in first out";
            }
            else if(line[i][1] == 'l' ){
                algorithm = "least recently used";
            }
        }
        else if(strlen(line[i]) < 3){//line[i][1] == 'f'){
            printf("%d\n", atoi(line[1]));
            numFrames = atoi(line[i]);
        }
    }
}

int main(int argc, char **argv){
    if(argc < 3 || argc > 4){ // early fail condition
        printf("Usage: ./memSwap # -(one of; -f, -l) < fileName.txt (optional)\n");
        return EXIT_FAILURE;
    }
    decipher(argv);
    printf("algorithm: %s, numFrames: %d\n", algorithm, numFrames);
}