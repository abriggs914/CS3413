#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

const long int offsetMask = (pow(2,12)-1), pageMask = pow(2,12);

int main(int argc, char ** argv){
  if(argc < 2 || argc > 3){
    printf("Usage: ./addressConvert # FILENAME.txt (optional)\n");
    return EXIT_FAILURE;
  }
  long int num, count = 0;
  int y = 500;
  char numS[32];
  long int pageTable[(int)pow(2,20)][2];
  while(count < 10){
    printf("\n\n\n");
    scanf("%ld\n", &num);
    sprintf(numS, "%d", num);
    printf("numS: %s, strlen(numS): %d\n", numS, strlen(numS));
    printf("num: %ld, num/pageMask: %ld, num&offsetMask: %ld\n", num, (num/pageMask), (num&offsetMask));
    count++;
  }
}
