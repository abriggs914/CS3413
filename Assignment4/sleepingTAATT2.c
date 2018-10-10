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

struct job * front; // pointer to the front of the chair queue
struct job * rear; // pointer to the last of the chair queue
int ta[4];
int c; // num chairs, queue is 4 + c long.

pthread_mutex_t mutex[5] = PTHREAD_MUTEX_INITIALIZER;

pthread_t pta[4];//aid, bid, cid, did; // 4 TAs (a-d)id
int num = 0, studentNumber = 0;

void * foo(void * taIDIn){

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

void removeJob(int offset, struct job * frontIn){
	struct job * temp = frontIn;
	struct job * temp2 = frontIn;
	struct job * firstChair = frontIn;
	struct job * placeHolder = (struct job *) malloc(sizeof(struct job));
	placeHolder->studentID = -900;
	placeHolder->duration = -900;
    placeHolder->next = NULL;
	int i, j = length(frontIn);
	
	printf("offset : %d, j : %d\n", offset, j);
	if(offset == 0 && j == 1){ // remove first and only job
		printf("CASE 1\n");
		front = NULL;
	}
	else if(j < 5){ // nothing to replace what is removed
		printf("CASE 2\n");
		while(offset > 0){
			temp = temp->next;
			offset--;
		} // locate the job to remove
		placeHolder->next = temp->next;
		temp = placeHolder;
	}
	else{ // a job is waiting
		printf("CASE 3\n");
		for(i = 0; i < 4; i++){
			firstChair = firstChair->next;
		}
		if(temp->next != NULL){
			temp2 = firstChair->next;
		    firstChair->next = temp->next;
		printf("got here\n");
			i = 0;
		    while(firstChair->next != temp2 && i < j){
		    	firstChair = firstChair->next;
		    	i++;
		    }
		    firstChair->next = temp2;
		}
		temp = firstChair;
	}
	/*
	//struct job * tempNxt;
	//printf("got here\n");
	while(offset >= 1 && temp != NULL){
		temp = temp->next;
		offset--;
	}
	if(j > 4){
		for(i = 0; i < 5; i++){
			firstChair = firstChair->next;
		}
		if(temp->next != NULL){
		    firstChair->next = temp->next;
		}
	}
	else{
		printf("j > 4\n");
		if(temp->next != NULL){
		    placeHolder->next = temp;
		}
		else if(firstChair->next != NULL){
			placeHolder->next = firstChair->next;
		}
		firstChair = placeHolder;
	}
	printf("offset : %d, j : %d\n", offset, j);
	temp = firstChair;
	free(placeHolder);
	//temp->next = temp->next->next;*/
	printf("job removed\n");
}

int enQueue(int idIn, int qlen) {
	struct job * link = (struct job *) malloc(sizeof(struct job));
	link->studentID = idIn;
	link->duration = qlen;
	int i = 0, j;
	//printf("idIn: %d, qlen: %d\n", idIn, qlen);
    link->next = NULL;
	if(front == NULL && rear == NULL){ // queue is empty
		front = rear = link;
		return 1; // success
	}
	j = length(front);
	if(j == 4 + c){
		return 0; // fail // not enough chairs or TAs
	}
	rear->next = link;
	rear = link;
	//printf("j: %d\n", j);
	if(j == 0){
		j++;
	}
	return j; // success
}
/*
// To Dequeue the first student.
void deQueue() {
	struct job * temp = front;
	if(front == NULL) {
		printf("Queue is Empty\n");
		return;
	}
	if(front == rear) {
		front = rear = NULL;
	}
	else {
		front = front->next;
	}
	free(temp);
}*/

int main(int argc, char ** argv){
	int t, n, i, j, numStudents, temp, p, q, tsec;
	bool acceptStudent = true;
	struct job * tjob;
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
	printf("\tc: %d\n\tt: %d\n\tn: %d\n\tnum: %d\n", c, t, n, rand() % n);
	tsec = 0;
	i = 0;
	while(acceptStudent){
		if(tsec % numStudents == 0 && tsec != 0){ // create a student
			q = rand() % 30;
			q = ((q < 3)? q += 3 : q);
			printf("Spawn student: %d t(%d), with qlen: %d\n", i+1, tsec, q);
			p = enQueue(i+1, q);
			if(p == 0){
				printf("\tStudent %d is not taking a chance in the lab with their assignment.\n", i);
			}
			else if(p >= 4){
				printf("\tStudent %d is waiting for a TA.\n", i+1);
			}
			i++;
		}
		if(front == NULL){
			printf("...waiting for a student to arrive...\n");
			//sleep(1);
		}
		else{
			pthread_mutex_lock(&mutex[4]);
			j = length(front);
			temp = 0;
			int frontoffset = 0;
			struct job * curr = front;
			while(j > 0 && temp < 4){ // j checks end of queue
				if(curr->duration != -900){
					ta[temp]--;
					curr->duration--;
					printf("Student %d is working with TA[%d] for %d more sec\n", curr->studentID, temp, curr->duration);
					if(curr->duration <= 0){
						printf("removeJob\n");
						removeJob(frontoffset, front);
					}
				}
				else{
					printf("PLACEHOLDER\n");
				}
				temp++;
				frontoffset++;
				j--;
				if(j > 0){
					curr = curr->next;
				}
			}
			pthread_mutex_unlock(&mutex[4]);
		}
		tsec++;
		bool taGoHome = false;
		j = 0;
		q = 0;
		while(j < 4){
			if(ta[j] <= 0){
				//pthread_mutex_lock(&mutex[j]);
				q++;
			}
			j++;
		}
		if(q == 4 || tsec > 500){
			if(tsec > 500){
				printf("poor constraint\n");
			}
			acceptStudent = false;
			j = 0;
			while(j < 4){
				printf("ta[%d]: %d\n", j, ta[j]);
				j++;
			}
		}
	}
	/*
	while(acceptStudent && tsec < 500){
		ta[0] -= 100;
		ta[1] -= 100;
		ta[2] -= 100;
		ta[3] -= 100;
		p = rand() % 30;
		p = ((p < 3)? p += 3 : p);
		insertFirst(++studentIDs, p);
		for(p = 0; p < 4; p++){
			if(ta[p] == 0){
				q++;
			}
		}
		if(q == 4){
			acceptStudent = false;
		}
		q = 0;
		tsec++;
		printf("tsec: %d\n", tsec);
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
		pthread_join(pta[i], NULL);
	}
	return EXIT_SUCCESS;
}
