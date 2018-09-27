/*
*	C program to read in a list of users,
*	processes, their arrival times, and
*	their expected runtime. Program also 
*	takes in a number of available CPUs to
*	allow for multi-core processing.
*	Each CPU is simulated by a pthread.
*	Program follows	a SJF (shortest job first)
*	algorithm for selecting a process to
*	pass to the CPU.
*	
*	CS3413 Assignment 3
*	Oct 2018
*	Avery Briggs
*	3471065
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef int bool;
#define true 1
#define false 0

/*
	Process struct
	user - user name
	processID - processName given
	arrivalT - time process arrived to queue
	durationT - expected process runtime
	arrivalOrder - individual order of process arrivals
	lastOfUserProcessesFinishT - Time at which last process owned by particular user finishes
	next - points to the next process in line
*/
struct node{
	char * user;
    char processID;
    int arrivalT;
    int durationT;
    int arrivalOrder;
    int lastOfUserProcessesFinishT;
    struct node * next;
};

struct node * head = NULL; // start of the queue list.
struct node * withCPU = NULL; // start of the place holder list.
struct node * finishedProcesses = NULL; // start of the finished

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//Function prints the contents of the list in a formatted text
void printList(struct node * headIn){
   struct node * ptr = headIn;
   while(ptr != NULL){
      printf("\t%s, %c, AT:%d, DT:%d, AO:%d, LP:%d\n",ptr->user, ptr->processID, ptr->arrivalT, ptr->durationT, ptr->arrivalOrder, ptr->lastOfUserProcessesFinishT);
      ptr = ptr->next;
   }
}

void wereFree(struct node * headIn){
	struct node * temp1 = headIn;
	struct node * temp2;	
	while(temp1 != NULL){
		temp2 = temp1;
		free(temp1->user);
		free(temp1);
		temp1 = temp2->next;
	}
}

// Function prints the attributes of a processNode.
void printProcessNode(struct node * ptr){
      printf("\t%s, %c, AT:%d, DT:%d, AO:%d, LP:%d\n",ptr->user, ptr->processID, ptr->arrivalT, ptr->durationT, ptr->arrivalOrder, ptr->lastOfUserProcessesFinishT);
}

//inserts a link at the first of the list given process parameters
void insertFirst(char * userIn, char processIDIn, int arrivalIn, int durationIn, int arrivalOrder){
    struct node * link = (struct node *) malloc(sizeof(struct node));
    link->user = malloc(20*sizeof(char));
    strcpy(link->user, userIn);
    link->processID = processIDIn;
    link->arrivalT = arrivalIn;
    link->durationT = durationIn;
	link->arrivalOrder = arrivalOrder;
    link->next = head;
    head = link;
}

// Function inserts a given node into the head
// of the queue.
// This function is equivalent to insertFirst(), 
// except it is simplified for later calls.
// Function is only called when a process comes
// back from CPU and still has a durationT > 0.
void insertFirstNode(struct node * nodeIn){
	//printf("insertFirstNode: %c\n", nodeIn->processID);
	struct node * link = (struct node *) malloc(sizeof(struct node));
    link->user = nodeIn->user;
    link->processID = nodeIn->processID;
    link->arrivalT = nodeIn->arrivalT;
    link->durationT = nodeIn->durationT;
	link->arrivalOrder = nodeIn->arrivalOrder;
	link->lastOfUserProcessesFinishT = nodeIn->lastOfUserProcessesFinishT;
    link->next = head;
    head = link;
}

// Function inserts a processNode into 
// the head of the CPU list.
// This list is a place holder for when a 
// processes is with a CPU.
void insertFirstNodeCPU(struct node * nodeIn){
	//printf("insertFirstNodeCPU: %c\n", nodeIn->processID);
	struct node * link = (struct node *) malloc(sizeof(struct node));
    link->user = nodeIn->user;
    //printf("made it here\n");
    link->processID = nodeIn->processID;
    link->arrivalT = nodeIn->arrivalT;
    link->durationT = nodeIn->durationT;
	link->arrivalOrder = nodeIn->arrivalOrder;
	link->lastOfUserProcessesFinishT = nodeIn->lastOfUserProcessesFinishT;
    link->next = withCPU;
    withCPU = link;
}

// Function inserts a given node into the 
// head of finishedProcesses.
// Function is called when a process is finished
// processing and will be removed from the queue
// permanently.
void insertFinishedNode(struct node * nodeIn, int LPFT){
	//printf("insertFirstNodeCPU: %c\n", nodeIn->processID);
	struct node * link = (struct node *) malloc(sizeof(struct node));
    link->user = nodeIn->user;
    //printf("made it here\n");
    link->processID = nodeIn->processID;
    link->arrivalT = nodeIn->arrivalT;
    link->durationT = nodeIn->durationT;
	link->arrivalOrder = nodeIn->arrivalOrder;
	link->lastOfUserProcessesFinishT = LPFT;
    link->next = finishedProcesses;
    finishedProcesses = link;
}

// Function swaps processNodes attributes.
// Called from the sort() function.
void swapProcesses(struct node * current, struct node * next){
	int tempArrival, tempDuration, tempArrOrd, tempLPT;
 	char tempProcess;
    char * tempUser;
	//swap user
    tempUser = current->user;
    current->user = next->user;
    next->user = tempUser; 
    //swap ProcessID
	tempProcess = current->processID;
	current->processID = next->processID;
	next->processID = tempProcess; 
	//swap arrivalT   
	tempArrival = current->arrivalT;
	current->arrivalT = next->arrivalT;
	next->arrivalT = tempArrival;
	//sawp durationT
	tempDuration = current->durationT;
	current->durationT = next->durationT;
    next->durationT = tempDuration;
	//swap arrivalOrder
	tempArrOrd = current->arrivalOrder;
	current->arrivalOrder = next->arrivalOrder;
	next->arrivalOrder = tempArrOrd;
	//swap lastOfUserProcessesFinishT
	tempLPT = current->lastOfUserProcessesFinishT;
	current->lastOfUserProcessesFinishT = next->lastOfUserProcessesFinishT;
	next->lastOfUserProcessesFinishT = tempLPT;
}

// Function calculates and returns the length of a given list
int length(struct node * headIn){
    int length = 0;
    struct node * current;
    for(current = headIn; current != NULL; current = current->next){
		length++;
    }
    return length;
}

/* 
	Function arranges the list of processes based on the SJF algorithm
    function takes in time t to determine whether a process is eligible
    to be processed, then sorts by shortest job.
*/
void sort(int t){
	int i, j, k;
    struct node * current;
    struct node * next;	
    int size = length(head);
    k = size;	
    for(i = 0 ; i < size - 1 ; i++, k--){
    	current = head;
        next = head->next;
        for(j = 1 ; j < k ; j++){
        	if(current->arrivalT <= t && next->arrivalT <= t){
            	if(current->durationT > next->durationT){
        			swapProcesses(current, next);
        		}
			}
         	else if(current->arrivalT > next->arrivalT){
        		swapProcesses(current, next);
			}
         	current = current->next;
        	next = next->next;
		}
	}  
}
// Function is called to arrange 
// finishedProcesses list by processID.
// Assuming all user processes will have
// a sequential ID.
void sortFinishedProcesses(){
	int i, j, k;
    struct node * current;
    struct node * next;	
    int size = length(finishedProcesses);
    k = size;	
    for(i = 0 ; i < size - 1 ; i++, k--){
    	current = finishedProcesses;
        next = finishedProcesses->next;
        for(j = 1 ; j < k ; j++){ 
        	if(current->processID > next->processID){
        		swapProcesses(current, next);
			}
         	current = current->next;
        	next = next->next;
		}
	}   
}


// Function deletes the first
// procesNode from the list.
// Called to allow another CPU
// to look at a "new" head.
// Returns a pointer to the 
// deleted node so it may be
// appended if more processing
// is required.
struct node * deleteFirst() {
   struct node * link = head;
   head = head->next;
   return link;
}

//Function swaps two given integer pointers
void swap(int * a, int * b){
    int c = * a;
    * a = *b;
    * b = c;
}

// Function is called to go through finishedProcesses
// list, and update all processes owned by that
// user to the most recent finish time.
void updateUserFinishTime(struct node * temp, int t){
	char * userIn = temp->user;
	struct node * link = finishedProcesses;
	while(link != NULL){
		if(strcmp(userIn, link->user) == 0){
			link->lastOfUserProcessesFinishT = t;
		}
		link = link->next;
	}
}

void * foo(){	
	printf("\tpthread ID (FOO): %d\n\n", getpid());
	pthread_exit(0);
}

void * processJob(void * arg){
	struct node * temp = (struct node *) arg;
	//int num = *((int *) arg);
	//printf("num: %d\n", num);
	//printf("User name: %s\n", ((struct node *)arg)->user);
	pthread_mutex_lock(&mutex);
	temp->durationT--;
	printProcessNode(temp);
	//num--;
	pthread_mutex_unlock(&mutex);
	foo();
	pthread_exit(0);
}

int main(int argc, char ** argv){
    int h, q = 1;
    int numCPUs;
    numCPUs = atoi(*(argv+1));  // read in from command line
    //printf("number of CPUs available entered: \n\t%d\n\nargv: %s\targc: %d, argv[1][0]: %s\n\n", numCPUs, *argv, argc, *(argv+1));
    
	char title[100];
    char userIn[25];
    char process;
    char space;
    int arrival;
    int duration;
    scanf("%[^\n]", title);
    h = scanf("%s", userIn);
    while(h == 1){	// reading in processes 1 by 1 and creating list nodes
        scanf("%c", &space); // reading tab
    	scanf("%c", &process);
    	scanf("%d", &arrival);
    	scanf("%d", &duration);
        insertFirst(userIn, process, arrival, duration, q);
		q++;
    	h = scanf("%s", userIn);
    }
    sort(0); //in arrival order (t == 0)
    //printf("\n\tprinting initially:\n");
    //printList(head);
    int i, j, k = 0, t = 0;
    bool processList = (numCPUs > 0 && head != NULL);
	printf("Time\t");
	for(i = 0; i< numCPUs; i++){
		printf("CPU%d\t", (i+1));
	}
	printf("\n");
	struct node * temp;
	bool adjust = false;
	while(processList){	//list is not null and at least one processor available
		t++;
		temp = head;
		//////////////////////////////////////////////
		while(k < numCPUs && head != NULL){ //one t up to numCPUs being processed at once
			if(head->arrivalT <= t){
				if(k == 0){
					printf("%d\t", t);
				}
				j = head->durationT-1;
				head->durationT = j;
				printf("%c\t", head->processID);
				if(head->durationT <= 0){ //process is done
					temp = deleteFirst();
					updateUserFinishTime(temp, t+1);
					insertFinishedNode(temp, t+1);
				}
				else{ //pull process off list to give new head to the next CPU
					temp = deleteFirst();
					head = temp->next;
					insertFirstNodeCPU(temp);
					adjust = true;
				}
			}
			k++;
		}
		////////////////////////////////////////////////////
		
		printf("\n");
		k = 0;
		if(adjust){
			temp = withCPU;
			while(temp != NULL && k < numCPUs){
				insertFirstNode(temp);
				temp = temp->next;
				k++;
			}
			k = 0;
			withCPU = NULL;
			adjust = false;
		}
		sort(t+1);
		if(head == NULL){
			processList = false;
		}
		////////////////////////////////////////////////////
		if(processList){ 
			struct node * list = malloc(length(head)*sizeof(struct node *));
			list = head;
			pthread_t * pid = (malloc(sizeof(pthread_t)));
			pthread_create(pid, NULL, &processJob, list);
			printf("\tpthread ID (MAIN): %ld\n", *pid);
			printProcessNode(head);
			printf("\t\tVS.\n");
			pthread_join(*pid, NULL);
		}
		//need to implement this for multiple concurrent processes.
		////////////////////////////////////////////////////
	}
	printf("%d\tIDLE\n", (t+1));
	sortFinishedProcesses(); // finishedProcesses in order of processID
	//printList(finishedProcesses);
	temp = finishedProcesses;
	printf("\nSummary:\n");
	i = length(finishedProcesses);
	j = temp->lastOfUserProcessesFinishT;
	while(i > 0){ 
		printf("%s\t%d\n", temp->user, temp->lastOfUserProcessesFinishT);
		while(temp != NULL && temp->lastOfUserProcessesFinishT == j){
			temp = temp->next;
			i--;
		}
		if(temp == NULL){
			break;
		}
		j = temp->lastOfUserProcessesFinishT; 
	}
	/*
	for(i = 0; i < (t+2); i++){
		temp = finishedProcesses;
		while(temp != NULL){
			if(temp->lastOfUserProcessesFinishT == i){
				printf("%s\t%d\n", temp->user, temp->lastOfUserProcessesFinishT);
				break;
			}
			else{
				temp = temp->next;
			}
		}
	}*/
	free(temp);
	wereFree(head);
	wereFree(withCPU);
	wereFree(finishedProcesses);
	return EXIT_SUCCESS;
}
