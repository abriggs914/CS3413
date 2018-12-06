#include <stdio.h>
#include <stdlib.h>

void place(int arr[], int num);

typedef struct Node{
  int sector;
  int nextSector;
  struct Node * next;
  int length;
};

// typedef struct List{
//   int length;
//   struct Node * next;
// };

int runner;
struct Node * head;

int main(){
  int i = 0, scan, j = 0, k = 0, max = 0, narrH = 0;
  int a;
  int b;
  // int arr[10000][3];
  // int * res = malloc(10000*sizeof(int));
  int arr[25][3];
  // int res[25];
  int * res = malloc(25*sizeof(int));
  scan = scanf("%d %d",&a,&b);
  runner = 0;
  struct Node narr[25]; // = malloc(25*sizeof(struct Node *));
  // for(i = 0; i < 25; i++){
  //     narr[i] = NULL;
  // }
  while(scan == 2){
    arr[i][0] = a;
    arr[i][1] = b;
    if(b == -1){
      arr[i][0] = 1; // false
    }
    // printf("a: %d, b: %d\n", a,b);
    scan = scanf("%d %d",&a,&b);
    i++;
  }
  // for(i = 0; i < 10000; i++){
  struct Node temp;
  for(i = 0; i < 25; i++){
    temp.sector = i;
    temp.nextSector = arr[i][1];
    temp.length = k;
    temp.next = NULL;
    narr[narrH] = temp;
    j = i;
    k = 0;
    if(arr[i][1] != -1){
      printf("{ %d : %d } -> ", i, arr[i][1]);
      place(res, i);
      j = i;
      i = arr[i][1];
      while(arr[i][1] != -1){
        k++;
        printf("{ %d : %d } -> ", i, arr[i][1]);
        temp.sector = i;
        temp.nextSector = arr[i][1];
        temp.length = k;
        temp.next = NULL;
        narr[narrH].next = &temp;
        narr[narrH] = *narr[narrH].next;
        place(res, i);
        i = arr[i][1];
      }
      k += 2;
      temp.length = k;
      printf("{ %d : %d }, K [%d]\n\n", i, arr[i][1], k);
      place(res, i);
      printf("----------------------\n");
      if(k > max){
        max = k;
      }
    }
    // printf("Next Line\n");
    i = j;
    narrH++;
    // if(i > 5){
    //     break;
    // }
  }
  printf("Largest K: %d\n", max);
  for(i = 0; i < runner; i++){
    printf("arr[%d]: %d\n", i, res[i]);
  }
  for(i = 0; i < runner; i++){
    printf("narr[%d]: %d\n", i, narr[i].sector);
  }
}


void place(int *arr, int num){
    int i;
    // printf("runner: %d, num: %d", runner, num);
    for(i = 0; i < runner; i++){
        if(arr[i] == num){
            // printf(" failed\n");
            return;

      }
    }
    // printf(" runner: %d, num: %d", runner, num);
    // printf(" passed");
    arr[runner] = num;
    // printf(" runner: %d, num: %d, arr[runner]: %d\n", runner, num, arr[runner]);
    runner++;
}
