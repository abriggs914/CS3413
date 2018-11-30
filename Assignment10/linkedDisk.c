/*
*   CS3413 Assignment 10
*   Dec 2018
*   Avery Briggs
*   3471065
*
*   C program to simulate the disk rotation and request 
*   servicing over time. Program uses one of four algorithms
*   to simulate execution; FCFS, SSTF, CSCAN and, LOOK.
*   Program outputs the total disk reader movements, and 
*   time it took to service all requests. Command line input
*   must be ./linkedDisk (one of: F T C L) # < fileName.txt
*   Requests are expected to be in chronilogical order.
*   Sector input should be between 0-9999. Time may be
*    between 0-2^32.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef int bool;
#define true 1
#define false 0

// Struct to represent a request
// sector   - where the request needs to be serviced
// arrival  - the earliest time the request can be services
// next     - points to the next request in queue
typedef struct request{
    int sector;
    unsigned int arrival;
    struct request *next;
};

int head; // location of disk head reader
int totalHeadMovement; // running total of disk movements
double time; // running time
char * algorithm; // user selected algorithm
bool directionFroward = true; // indicates the direction of disk rotation. true == counterclockwise
struct request * front;

// Function prints the attributes of a paramertized request
void printReq(struct request * req){
    printf("\tRequest: Go to sector: %d, arrived at time: %u\n", req->sector, req->arrival);
}

// Function prints the list of requests
void printReqList(){
    struct request * temp = front;
    printf("\t\tLIST<\n");
    while(temp != NULL){
        printReq(temp);
        temp = temp->next;
    }
    printf("\t\t>LIST\n");
}

// Function is called at the beginning of exection
// to assign values to head and algorithm. It also
// initializes time and totalHeadMovements to 0.
void init(char * arr[]){
    time = 0;
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


// Function creates a new request node for the linked
// list, and queues it.
void enqueue(int s, unsigned int t){
    struct request * temp = (struct request *) malloc(sizeof(struct request));
    temp->sector = s;
    temp->arrival = t;
    struct request * tmp = front;
    if(tmp == NULL){
        front = temp;
    }
    else{
        while(tmp->next != NULL){
            tmp = tmp->next;
        }
        tmp->next = temp;
    }
}

// Function removes the paramertized request from
// the list of requests.
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
    tmp->next = tmpNext->next;
}

// Function deciides whether to change the direction of
// the disk. Only called by look to allow it to turn
// around before hitting the boundry.
bool switchDirection(){
    struct request * temp = front;
    while(temp != NULL){
        if(directionFroward && temp->sector > head){
            return false;
        }
        else if(!directionFroward && temp->sector < head){
            return false;
        }
        temp = temp->next;
    }
    return true;
}

// Function is called to calculate the time it will
// take to service a request. It will add 5 s to the
// time if the shortest route will involve changing
// it's rotation. It also adjusts the totalHeadMovements
// based on the distance form the head to the sector.
double timeAdjust(int dest, bool adjust){
    int dist = abs(dest - head);
    if(dist > 500){
        dist = 10000 - dist;
    }
    double t = ((double)abs(dist)/(double)10);
    if((dest < head && directionFroward) || (dest > head && !directionFroward)){
        t += 5;
        directionFroward = !directionFroward;
    }
    if(adjust){
        totalHeadMovement += dist;
    }
    return t;
}

// Function implements the CSCAN algorithm.
// It returns a double of how much time it
// will take to service the request.
double fcfs(){
    double t = timeAdjust(front->sector, true);
    return t;
}

// Function implements the SSTF algorithm.
// It selects a new front request, and returns
// a double of how much time it will take to 
// service the request.
double sstf(){
    struct request * shortestTarget = front;
    struct request * temp = front;
    struct request * temp2;
    int shortestSeekTime = timeAdjust(front->sector, false);
    double t;
    while(temp->next != NULL){
        temp = temp->next;
        t = timeAdjust(temp->sector, false);
        if(temp->arrival <= time && (shortestSeekTime > t)){
            shortestTarget = temp;
            shortestSeekTime = t;
        }
    }
    temp = front;
    if(temp != shortestTarget){
        while(temp->next != shortestTarget){
            temp = temp->next;
        }
        temp2 = shortestTarget;
        temp->next = shortestTarget->next;
        temp2->next = front;
        front = shortestTarget;
    }    
    timeAdjust(front->sector, true);
    return t;
}

// Function implements the CSCAN algorithm.
// It selects a new front request, and returns
// a double of how much time it will take to 
// service the request.
double cscan(){
    double t, leftOver = 0;
    struct request * temp = front;
    struct request * ShortestTarget = front;
    int dist;
    int shortestDist;
    bool pass = true;
    shortestDist = 10000;
    directionFroward = true;
    while(temp != NULL){
        dist = abs(temp->sector - head);
        if(temp->sector < head){
            dist += 9999-head;
        }
        if(temp->arrival <= time){
            if(directionFroward && temp->sector > head && dist < shortestDist){
                ShortestTarget = temp;
                shortestDist = dist;
                pass = false;
            }
        }
        temp = temp->next;
        if(pass && temp == NULL){
            leftOver += timeAdjust(10000, false);
            temp = front;
            head = 0;
        }
    }
    dequeueReq(ShortestTarget);
    temp = front;
    ShortestTarget->next = temp;
    front = ShortestTarget;
    t = timeAdjust(front->sector, true);
    t += leftOver;
    return t;
}

// Function implements the LOOK algorithm.
// It selects a new front request, and returns
// a double of how much time it will take to 
// service the request.
double look(){
    double t;
    struct request * temp = front;
    struct request * ShortestTarget = front;
    int dist;
    int shortestDist;
    shortestDist = 10000;
    while(temp != NULL){
        dist = abs(temp->sector - head);
        if(switchDirection()){
            directionFroward = !directionFroward;
        }
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
    temp = front;
    ShortestTarget->next = temp;
    front = ShortestTarget;
    t = timeAdjust(front->sector, true);
    return t;
}

// Function service is called iteratively from main
// to constantly select the best and most up to date 
// request. Each algorithm will sort the list of 
// requests, putting the selected candidate in the
// front of the list. Then updating the time with 
// the returned service time, and head with the new
// sector. Finally it deQueues the request leaving
// behind only the unserviced requests.
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
    else{
        t = look();
    }
    time += t;
    head = front->sector;
    dequeueReq(front);
}

int main(int argc, char ** argv){
    printf("main\n");
    if(argc != 4){
        printf("Usage: ./linkedDisk (one of: F T C L) # < fileName.txt\n");
        return EXIT_FAILURE;
    }
    init(argv);
    if(algorithm == "UNDEFINED"){
        printf("Usage: ./linkedDisk (one of: F T C L) # < fileName.txt\n");
        return EXIT_FAILURE;
    }
    bool timePass = true, begin = true;
    int sectorIn, i;
    unsigned int timeIn;
    printf("head: %d, algorithm: %s\n", head, algorithm);
    i = scanf("%d %u", &sectorIn, &timeIn);
    if(i != 2){
        timePass = false;
    }
    while(timePass){
        if(i == 2){ // read something to enqueue
            printf("Request In: sector: %d, time: %u\n", sectorIn, timeIn);
            enqueue(sectorIn, timeIn);
            i = scanf("%d %u", &sectorIn, &timeIn);
        }
        // something queued and it is ready to be serviced
        if(front != NULL && front->arrival <= time && i != 2){
            if(begin){
                printReqList();
                begin = false;
            }
            service();
        }
        // nothing queued or nothing read in and waiting to service first request
        else if(front == NULL || (i != 2 && front->arrival > time)){
            time += 1;
        }
        printf("Time: %.1f s, Head: %d\n", time, head);
        // nothing queued and nothing read in
        if(front == NULL && i != 2){
            timePass = false;
        }
    }
    printf("\n\tSUMMARY\n");
    printf("Total Head Movements required: %d\n", totalHeadMovement);
    printf("Total time: %f\n", time);
}