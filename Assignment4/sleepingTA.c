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

int studentIDs = 0; // incremented for number of students
struct job * head; // pointer to the chair queue
int aTAT = 0, bTAT = 0, cTAT = 0, dTAT = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int num = 0, studentNumber = 0;

void * foo(){
	pthread_mutex_lock(&mutex);
	printf("\npthread_ID: %lld, num: %d\n", (long long)pthread_self(), num);
	num++;
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

void insertFirst(int idIn, int qlen){
	struct job * link = (struct job *) malloc(sizeof(struct job));
	link->studentID = idIn;
	link->duration = qlen;
	printf("idIn: %d, qlen: %d\n", idIn, qlen);
    link->next = NULL;
    head = link;
}

int main(int argc, char ** argv){
	int c, t, n, i, numStudents, temp, p, tsec;
	bool acceptStudent = true;
	struct job * tjob;
	time_t tm; 
	srand((unsigned) time(&tm));
	if(argc != 7){
		printf("Usage: 'a.out -c 5 -t 200 -n 12'\n\tPlease try again.\n");
		return 0;
	}
	c = atoi(argv[2]);
	t = atoi(argv[4]);
	n = atoi(argv[6]);
	aTAT = t;
	bTAT = t;
	cTAT = t;
	dTAT = t;
	if(t <= 0 || t <= 0){
		printf("Usage: 'a.out -c 5 -t 200 -n 12'\n\tPlease try again.\n");
		return 0;
	}
	numStudents = rand() % n; //numStudents (student per (n) seconds)
	printf("Number of students for extra help: %d\n", numStudents);
	printf("\tc: %d\n\tt: %d\n\tn: %d\n\tnum: %d\n", c, t, n, rand() % n);pthread_t ta[4];//aid, bid, cid, did; // 4 TAs (a-d)id
	for(i = 0; i < 4; i++){
		pthread_create(&ta[i], NULL, foo, NULL);
	}
	temp = numStudents;
	while(acceptStudent){
		aTAT -= 100;
		bTAT -= 100;
		cTAT -= 100;
		dTAT -= 100;
		p = rand() % 30;
		p = ((p < 3)? p += 3 : p);
		insertFirst(++studentIDs, p);
		if(aTAT == 0 && bTAT == 0 && cTAT == 0 && dTAT == 0){
			acceptStudent = false;
		}
	}

/*
	while(temp > 0){ // loops for a set number of students, doesnt work
		//pthread_mutex_lock(&mutex);  // for running TA's time down.
		p = rand() % 30;
		p = ((p < 3)? p += 3 : p);
		insertFirst(++studentIDs, p);	
		//pthread_mutex_unlock(&mutex);
		temp--;
	}*/
	
	for(i = 0; i < 4; i++){
		pthread_join(ta[i], NULL);
	}
	return EXIT_SUCCESS;
}
