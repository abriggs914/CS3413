#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int max(void * arr){
  int *a = (int *)arr;
  int s = sizeof(a);
  int c = sizeof(a);
  int f = 17;
  c = -1;
  for(s = 0; s < f; s++){
    if(a[s] > c){
      // printf("gothere, a[%d]: %d, f: %d\n",s,a[s],f);
      c = a[s];
    }
  }
  // printf("gothere, c: %d\n",c);
  return c;
}

int min(void * arr){
  int *a = (int *)arr;
  // int a[] = &b;
  int s = sizeof(a);
  int c = sizeof(a[0]);
  int f = 17;
  c = a[0];
  for(s = 0; s < f; s++){
    if(a[s] < c){
      // printf("gothere, a[%d]: %d, f: %d\n",s,a[s],f);
      c = a[s];
    }
  }
  // printf("gothere, c: %d\n",c);
  return c;
}

int avg(void * arr){
  int *a = (int *)arr;
  // int a[] = &b;
  int s = sizeof(a);
  int c = sizeof(a[0]);
  int f = 17;
  c = 0;
  for(s = 0; s < f; s++){
      c += a[s];
  }
  c = c/f;
  // printf("gothere, c: %d\n",c);
  return c;
}

int main (int argc, char ** argv){
  // int scan[] = getNums(argv);
  int arg[] = {1,2,3,4,2,8,7,6,-1,5,-6,5,8,8,2,3,9};
  int s = sizeof(arg);
  int c = sizeof(arg[0]);
  printf("s: %d, c: %d\n", s,c);
  int a = s/c, maxV = -1, minV = -1, avgV = -1;
  printf("a: %d\n", a);
  pthread_t workers[3];
  s = pthread_create(&workers[0],NULL,max,&arg);
  s = pthread_create(&workers[1],NULL,min,&arg);
  s = pthread_create(&workers[2],NULL,avg,&arg);
  s = pthread_join(workers[0], &maxV);
  s = pthread_join(workers[1], &minV);
  s = pthread_join(workers[2], &avgV);
  printf("maxV: %d\n", maxV);
  printf("minV: %d\n", minV);
  printf("avgV: %d\n", avgV);
  // char letterIn;
  // pthread_t pid[3];
  // scan = scanf("%c",letterIn);
  // while(scan == 1){
  //   pthread_create(&pid[0], &NULL, toLower(), &NULL);
  // }
}
