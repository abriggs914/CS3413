#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef int bool;
#define true 1
#define false 0

typedef struct request{
    int sector;
    unsigned int arrival;
    struct request *next;
};

int head;
int totalHeadMovement;
double time;
int requestInQueue;
char * algorithm;
bool directionFroward = true;

struct request * front;

void printReq(struct request * req);
void printReqList();
void init(char * arr[]);
void enqueue(int s, unsigned int t);
void dequeueReq(struct request * req);
double fcfs();
double sstf();
double cscan();
double look();
void service();

void printReq(struct request * req){
    printf("\tRequest: sector: %d, arrival: %u\n", req->sector, req->arrival);
}

void printReqList(){
    struct request * temp = front;
    printf("\t\tLIST<\n");
    while(temp != NULL){
        printReq(temp);
        temp = temp->next;
    }
    printf("\t\t>LIST\n");
}

void init(char * arr[]){
    time = 0;
    requestInQueue = 0;
    head = atoi(arr[2]);
    totalHeadMovement = 0;
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
        while(tmp->next != NULL){
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
        requestInQueue--;
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
    tmp->next = tmpNext->next;
    requestInQueue--;
}

double timeAdjust(int dest, bool adjust){
    int dist = abs(dest - head);
    if(dist > 500){
        dist = 1000 - dist;
    }
    double t = ((double)abs(dist)/(double)10);
    if((dest < head && directionFroward) || (dest > head && !directionFroward)){
        t += 5;
        directionFroward = !directionFroward;
    }
    printf("t: %f, hm: %d, dest: %d\n", t, totalHeadMovement, dest);
    if(adjust){
        totalHeadMovement += dist;
    }
    return t;
}

double fcfs(){
    double t = timeAdjust(front->sector, true);
    return t;
}

double sstf(){
    struct request * shortestTarget = front;
    struct request * temp = front;
    struct request * temp2; // = malloc(sizeof(struct request *));
    int shortestSeekTime = timeAdjust(front->sector, false);
    double t;
    while(temp->next != NULL){
        temp = temp->next;
        t = timeAdjust(temp->sector, false);
        if(temp->arrival <= time && (shortestSeekTime > t)){
            shortestTarget = temp;
            shortestSeekTime = t; //abs(temp->sector - head);
        }
        // printf("arrival: %d, time: %f, curr t: %f, shT->sector: %d, shT->arrival: %d\n",temp->arrival, time, t, shortestTarget->sector, shortestTarget->arrival);
    }
    temp = front;
    if(temp != shortestTarget){
        while(temp->next != shortestTarget){
            temp = temp->next;
        }
        temp2 = shortestTarget;
        temp->next = shortestTarget->next;
        temp2->next = front;
        // printReqList();
        // printReq(shortestTarget);
        front = shortestTarget;
        // printReqList();
    }    
    timeAdjust(front->sector, true);
    return t;
}

double cscan(){
    double t;
    struct request * temp = front;
    struct request * ShortestTarget = front;
    int dist;
    int shortestDist;
    shortestDist = 1000;
    printf("directionForward: %s\n", ((directionFroward)?"true":"false"));
    while(temp != NULL){
        dist = abs(temp->sector - head);
        // if(dist > 500){
        //     dist = 1000 - dist;
        // }
        printf("dist: %d, shortestDist: %d, temp->sector: %d\n", dist, shortestDist, ShortestTarget->sector);
        if(temp->arrival <= time){
            if(directionFroward && temp->sector > head && dist < shortestDist){
                ShortestTarget = temp;
                shortestDist = dist;
            }
            else if(!directionFroward && temp->sector < head && dist < shortestDist){
                ShortestTarget = temp;
                shortestDist = dist;
            }
        }
        temp = temp->next;
    }
    dequeueReq(ShortestTarget);
    printReq(ShortestTarget);
    temp = front;
    ShortestTarget->next = temp;
    front = ShortestTarget;
    printf("CSCNA");
    printReqList();
    t = timeAdjust(front->sector, true);
    return t;
}

void service(){
    double t;
    if(algorithm[0] == 'F'){
        t = fcfs();
    }
    else if(algorithm[0] == 'S'){
        t = sstf();
    }
    else if(algorithm[0] == 'C'){
        t = cscan();
    }
    time += t;
    head = front->sector;
    dequeueReq(front);
}

int main(int argc, char ** argv){
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
        if(i == 2){ // read something to enqueue
            printf("sectorIn: %d, timeIn: %u\n", sectorIn, timeIn);
            enqueue(sectorIn, timeIn);
            i = scanf("%d %u", &sectorIn, &timeIn);
        }
        if(front != NULL && front->arrival <= time && i != 2){ // something queued and it is ready to be serviced
            printReqList();
            service();
        }
        else if(front == NULL || (i != 2 && front->arrival > time)){ // nothing queued or (nothing read in and head arrriavl is > time)
            time += 1;
        }
        printf("Time: %f, Head: %d\n", time, head);
        if(front == NULL && i != 2){ // nothing queued and nothing read in
            timePass = false;
        }
    }
    // printReqList();
    // dequeueReq(front);
    // printReqList();
    // dequeueReq(front);
    // printReqList();
    // dequeueReq(front->next->next);
    printf("Total Head Movements required: %d\n", totalHeadMovement);
    printf("Total time: %f\n", time);
}