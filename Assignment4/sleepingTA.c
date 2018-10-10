/*
*
* CS3413 Assignment 4
* Oct 2018
* Avery Briggs
* 3471065
*
* C program to simulate the sleeping TA problem.
* There are 4 pthreads that represent TAs in the
* sunken lounge, waiting to help students with their
* assignments. Students will appear at a random until
* All TA's have reached their tutoring quota.
* Each student will have 1 question that will take
* a random amount of time between 3 and 30 seconds 
* of one TA's time. There are c chairs availible for
* students to queue while they wait for an available
* TA. Each TA has t seconds worth of tutor time in their contract.
* A TA will finish tutoring even if it takes them into OT.
* If there are no available TA's or chairs, the student
* leaves and doesn't return.
* The chair queue works in a FCFS manner.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef int bool;
#define true 1
#define false 0

/*
* Struct holds data for a students question.
* studentID - unique number given to students 
*	      	in order of arrival.
* arrival - the time in (t) seconds, of when 
		the student arrived.
* duration - 	how long it will take to answer
		their question. 
*/
struct job{	
	int studentID; 
	int arrival;
	int duration;
	struct job * next;
};


struct job * head[4]; // array for keeping track of which student is with which TA
struct job * front; // pointer to the front of the chair queue
struct job * rear; // pointer to the last of the chair queue
int ta[4];
int c; // num chairs in a queue, c long.

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t pta[4]; // 4 TAs 
int num = 0, studentNumber = 0;

void * foo(void * taIDIn){
	return NULL;
}

// Function calculates and returns the length of a given list
int length(struct job * headIn){
    int length = 0;
    struct job * current;
    for(current = headIn; current != NULL; current = current->next){
    	if(current->studentID != -900){
			length++;
		}
    }
    return length;
}

struct job * wakeupTA(int idIn, int qlen) {
	struct job * link = (struct job *) malloc(sizeof(struct job));
	link->studentID = idIn;
	link->duration = qlen;
    link->next = NULL;
	return link;
}

int enQueue(int idIn, int qlen) {
	struct job * link = (struct job *) malloc(sizeof(struct job));
	link->studentID = idIn;
	link->duration = qlen;
	int j;
	//printf("idIn: %d, qlen: %d\n", idIn, qlen);
    link->next = NULL;
	if(front == NULL && rear == NULL){ // queue is empty
		front = rear = link;
		return 1; // success
	}
	j = length(front);
	if(j == c){
		return 0; // fail not enough chairs or TAs
	}
	rear->next = link;
	rear = link;
	//printf("j: %d\n", j);
	if(j == 0){
		j++;
	}
	return j+4; // success
}

// To Dequeue the first student.
struct job * deQueue() {
	struct job * temp = front;
	if(front == NULL) {
		return NULL;
	}
	if(front == rear) {
		front = rear = NULL;
	}
	else {
		front = front->next;
	}
	return temp;
}

int main(int argc, char ** argv){
	int t, n, i, j, k, numStudents, p, q, tsec;
	bool acceptStudent = true;
	time_t tm; 
	srand((unsigned) time(&tm));
	if(argc != 7){ // not enough arguments
		printf("Usage: 'a.out -c 5 -t 200 -n 12'\n\tPlease try again.\n");
		return 0;
	}
	c = atoi(argv[2]);
	t = atoi(argv[4]);
	n = atoi(argv[6]);
	for(p = 0; p < 4; p++){
		ta[p] = t;
		head[p] = NULL;
	}
	if(t <= 0 || t <= 0){ // arguments out of bounds
		printf("Usage: 'a.out -c 5 -t 200 -n 12'\n\tPlease try again.\n");
		return 0;
	}
	for(i = 0; i < 4; i++){
		pthread_create(&pta[i], NULL, foo, NULL);
	}
	numStudents = rand() % n;
	while(numStudents == 0){
		numStudents = rand() % n; //numStudents (student per (n) seconds)
	}
	printf("1 student per %d seconds\n", numStudents);
	printf("\tc: %d\n\tt: %d\n\tn: %d\n\n", c, t, n);
	tsec = 0;
	i = 0;
	q = 0;
	while(acceptStudent){
		if(tsec % numStudents == 0 && tsec != 0){ // create a student	
			p = 0;
			while(q <= 2 || q >= 31){
				q = rand() % 30;
			}
			//q = ((q < 3)? q += 3 : q);
			//printf("Spawn student: %d t(%d), with qlen: %d\n", i+1, tsec, q);
			pthread_mutex_lock(&mutex);
			for(k = 0; k < 4; k++){
				if(head[k] == NULL){
					head[k] = wakeupTA(i+1, q);
					p=2;
					break;
				}
			}
			pthread_mutex_unlock(&mutex);
			if(k == 4){
				p = enQueue(i+1, q);
			}
			if(p == 0){
				printf("\tStudent %d is not taking a chance in the lab with their assignment.\n", i+1);
			}
			else if(p >= 4 || p == 1){
				printf("\tStudent %d is waiting for a TA.\n", i+1);
			}
			for(k = 0; k < 4; k++){
				if(head[k] != NULL){
					head[k]->duration--;
					ta[k]--;
				}
				if(head[k] != NULL && head[k]->duration <= 0){
					printf("\tStudent %d is going to pass this assignment!\n", head[k]->studentID);
    				head[k] = deQueue();
					if(ta[k] <= 0){
					}
				}
			}
			i++;
		}
		tsec++;
		j = 0;
		q = 0;
		while(j < 4){
			if(ta[j] <= 0){
				q++;
			}
			j++;
		}
		if(q == 4){ // all TAs have met quota
			acceptStudent = false;
			j = 0;
			while(j < 4){
				printf("ta[%d]: %d\n", j, ta[j]);
				j++;
			}
		}
	}
	for(i = 0; i < 4; i++){
		pthread_join(pta[i], NULL);
	}
	return EXIT_SUCCESS;
}
