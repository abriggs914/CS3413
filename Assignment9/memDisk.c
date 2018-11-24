#include <stdlib.h>
#include <stdio.h>

typedef int bool;
#define true 1
#define false 0

typedef struct request{
    int sector;
    unsigned int arrival;
    struct request *next;
};

int head;
int time;
int requestInQueue;
char *algorithm;

struct request *front;

void printReq(struct request * req){
    printf("Request: sector: %d, arrival: %u\n", req->sector, req->arrival);
}

void printReqList(){
    struct request * temp = front;
    printf("\tLIST<\n");
    while(temp != NULL){
        printReq(temp);
        temp = temp->next;
    }
    printf("\t>LIST\n");
}

void init(char *arr[]){
    time = 0;
    requestInQueue = 0;
    head = atoi(arr[2]);
    if(arr[1][0] == 'F'){
        algorithm = "First Come First Serve";
    }
    else if (arr[1][0] == 'T'){
        algorithm = "Shortest Seek Time First";
    }
    else if(arr[1][0] == 'C'){
        algorithm = "Circular SCAN";
    }
    else if(arr[1][0] == 'L'){
        algorithm = "LOOK";
    }
    else{
        algorithm = "UNDEFINED";
    }
}

void enqueue(int s, unsigned int t){
    struct request * temp = (struct request *) malloc(sizeof(struct request));
    temp->sector = s;
    temp->arrival = t;
    struct request * tmp = front;
    if(tmp == NULL){
        front = temp;
        requestInQueue++;
    }
    else{
        printf("else\n");
        int c = 0;
        while(tmp->next != NULL){
            printf("%d\n",c);
            c++;
            tmp = tmp->next;
        }
        tmp->next = temp;
        requestInQueue++;
    }
}

void dequeueReq(struct request * req){
    struct request * tmp = front;
    struct request * tmpNext = front;
    if(front == req){
        front = front->next;
        return;
    }
    if(tmp != NULL){
        tmpNext = tmp->next;
    }
    else{
        tmp = NULL;
        return;
    }
    while(tmpNext != req && tmpNext != NULL){
        tmp = tmpNext;
        tmpNext = tmpNext->next;
    }
    if(tmpNext == NULL){
        printf("HI\n");
    }
    printf("GOTHERE\n");
    tmp->next = tmpNext->next;
}

int main(int argc, char **argv){
    printf("main\n");
    if(argc > 4 || argc < 3){
        printf("Usage: ./memDisk (one of: F T C L)\n");
        return EXIT_FAILURE;
    }
    init(argv);
    if(algorithm == "UNDEFINED"){
        printf("Usage: ./memDisk (one of: F T C L)\n");
        return EXIT_FAILURE;
    }
    bool timePass = true;
    int sectorIn, i;
    unsigned int timeIn;
    printf("head: %d, algorithm: %s\n", head, algorithm);
    i = scanf("%d %u", &sectorIn, &timeIn);
    if(i != 2){
        timePass = false;
    }
    while(timePass){
        printf("time: %d\n", time);
        if(i == 2){ // read something to enqueue
            printf("sectorIn: %d, timeIn: %u\n", sectorIn, timeIn);
            enqueue(sectorIn, timeIn);
            i = scanf("%d %u", &sectorIn, &timeIn);
        }
        if ((i != 2 && front == NULL) || time > 5){ // nothing read in
            timePass = false;
        }
        time++;
    }
    printReqList();
    dequeueReq(front);
    printReqList();
    dequeueReq(front);
    printReqList();
    dequeueReq(front->next->next);
    printReqList();
}