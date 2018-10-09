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
* assignments. Students will appear at a random rate
* between 0 and n seconds. Each student will have 1
* question that will take a random amount of time
* between 0 and 30 seconds of one TA's time. There
* are c chairs availible for students to queue while
* they wait for an available TA. Each TA has t seconds 
* worth of tutor time in their contract.
* A TA will finish tutoring even if it takes them into OT.
* If there are no available TA's or chairs, the student
* leaves and doesn't return.
* The chair queue works in a FCFS manner.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int num = 0, studentNumber = 0;

void * foo(){
	pthread_mutex_lock(&mutex);
	printf("\npthread_ID: %lld, num: %d\n", (long long)pthread_self(), num);
	num++;
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

void insertFirst(int idIn){
	struct job * link = (struct job *) malloc(sizeof(struct job));
	link->studentID = idIn;
	printf("idIn: %d\n", idIn);
    link->next = NULL;
    head = link;
}

int main(int argc, char ** argv){
	int c, t, n, i, numStudents, temp;
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
	while(temp > 0){
		//pthread_mutex_lock(&mutex);
		insertFirst(++studentIDs);	
		//pthread_mutex_unlock(&mutex);
		temp--;
	}
	
	for(i = 0; i < 4; i++){
		pthread_join(ta[i], NULL);
	}
	return EXIT_SUCCESS;
}
