/*
*	C program to read in a list of users,
*	processes, their arrival times, and
*	their expected runtime. Program also 
*	takes in a number of available CPUs to
*	allow for multi-core processing.
*	Program follows	a SJF (shortest job first)
*	algorithm for selecting a process to
*	pass to the CPU.
*	
*	CS3413 Assignment 2
*	Sept 2018
*	Avery Briggs
*	3471065
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    int processed;  //1 yes, 0 not yet
    struct node * next;
};

struct nodeCPU{
	char currProcessID;
	int cpuID;
	bool inUse;
    struct nodeCPU * next;
};

// start of the list
struct node * head = NULL;
struct node * finishedProcesses = NULL;
struct nodeCPU * headCPU = NULL;

//Function prints the contents of the list in a formatted text
void printList(){
   struct node * ptr = head;
   while(ptr != NULL){
      printf("\t%s, %c, AT:%d, DT:%d, AO:%d, LP:%d, PD?:%d\n",ptr->user, ptr->processID, ptr->arrivalT, ptr->durationT, ptr->arrivalOrder, ptr->lastOfUserProcessesFinishT, ptr->processed);
      ptr = ptr->next;
   }
}
void printProcessNode(struct node * ptr){
      printf("\t%s, %c, AT:%d, DT:%d, AO:%d, LP:%d, PD?:%d\n",ptr->user, ptr->processID, ptr->arrivalT, ptr->durationT, ptr->arrivalOrder, ptr->lastOfUserProcessesFinishT, ptr->processed);
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
	link->processed = 0;
    link->next = head;
    head = link;
}

void insertFirstNode(struct node * nodeIn){
    //struct node * link = (struct node *) malloc(sizeof(struct node));
    //link->user = malloc(20*sizeof(char));
    //strcpy(link->user, userIn);
    //link->processID = processIDIn;
    //link->arrivalT = arrivalIn;
    //link->durationT = durationIn;
	//link->arrivalOrder = arrivalOrder;
	//link->processed = 0;
    nodeIn->next = head;
    head = nodeIn;
}

void insertFinishedProcess(char * userIn, char processIDIn, int arrivalIn, int durationIn, int arrivalOrder, int processedIn, int lastOfUserProcessesFinishIn){
    struct node * link = (struct node *) malloc(sizeof(struct node));
    link->user = malloc(20*sizeof(char));
    strcpy(link->user, userIn);
    link->processID = processIDIn;
    link->arrivalT = arrivalIn;
    link->durationT = durationIn;
	link->arrivalOrder = arrivalOrder;
	link->processed = processedIn;
    link->next = finishedProcesses;
    link->lastOfUserProcessesFinishT = lastOfUserProcessesFinishIn;
    finishedProcesses = link;
}

void swapProcesses(struct node * current, struct node * next){
	int tempArrival, tempDuration, tempArrOrd, tempLPT, tempPCD;
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
	//swap processed
	tempPCD = current->processed;
	current->processed = next->processed;
	next->processed = tempPCD;
}

// Function calculates and returns the length of the list
int length(){
    int length = 0;
    struct node * current;
    for(current = head; current != NULL; current = current->next){
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
    int size = length();
    k = size;	
    for(i = 0 ; i < size - 1 ; i++, k--){
    	current = head;
        next = head->next;
        for(j = 1 ; j < k ; j++){
        	//printf("\tt: %d \tcurrent: \n",t);
        	//printProcessNode(current);   
        	//printf("\n\tt: %d \tnext: \n",t);   
        	//printProcessNode(next);   
        	//printf("\n");   
        	if((current->arrivalT <= t && next->arrivalT <= t) || current->processed == 1){
            	if(current->durationT > next->durationT){
        			swapProcesses(current, next);
        		}
			}
         	else if(((current->arrivalT > next->arrivalT) || current->processed == 1)){
        		swapProcesses(current, next);
			}
         	current = current->next;
        	next = next->next;
		}
	}   
}

void updateRestOfUserProcesses(char * userIn, int n){
    struct node * link = head;
    while(link != NULL){
        if(strcmp(link->user, userIn) == 0){
            link->lastOfUserProcessesFinishT = n;
        }
        link = link->next;
    }
}

//Function simulates a CPU cycle, decreasing the duration left on
//a current process by 1.
int processJob(struct node * nodeIn, struct nodeCPU * cpuIn, int t, int numCPUs, int ref){
	struct node * currentlyExecuting = nodeIn;
	printProcessNode(currentlyExecuting);
    if(currentlyExecuting->arrivalT <= t){
		if(currentlyExecuting->durationT > 0){
		    currentlyExecuting->durationT--;
		    cpuIn->currProcessID = currentlyExecuting->processID;
		    cpuIn->inUse = true;
		    printf("%c\t%d\t",currentlyExecuting->processID, t);
		    if(currentlyExecuting->durationT == 0){
		        currentlyExecuting->processed = 1;  //using this as a check for when process is finished.
		    	currentlyExecuting->lastOfUserProcessesFinishT = t+1;
		    	updateRestOfUserProcesses(currentlyExecuting->user, (t+1));
		    	insertFinishedProcess(currentlyExecuting->user, currentlyExecuting->processID, currentlyExecuting->arrivalT, currentlyExecuting->durationT, currentlyExecuting->arrivalOrder, currentlyExecuting->processed, currentlyExecuting->lastOfUserProcessesFinishT);
		    }
		    nodeIn = currentlyExecuting;
		    /*if(nodeIn->next->processed == 0 && ref < numCPUs){
		    	processJob(nodeIn->next, cpuIn->next, t, numCPUs, ref+1);
		    }*/
		    return 1;
		}
    }
    return 0;
}

void processFinished(struct node * nodeIn, int t){

}

//delete first item
struct node * deleteFirst() {

   //save reference to first link
   struct node * link = head;
	
   //mark next to first link as first 
   head = head->next;
	
   //return the deleted link
   return link;
}

void reverse(struct node * head) {
   struct node * prev = NULL;
   struct node * current = head;
   struct node * next;
   while(current != NULL) {
      next = current->next;
      current->next = prev;   
      prev = current;
      current = next;
   }
   head = prev;
}

void initCPU(int cpuID){
    struct nodeCPU * link = (struct nodeCPU *) malloc(sizeof(struct nodeCPU));
    link->cpuID = cpuID;
    link->next = headCPU;
    link->inUse = false;
    headCPU = link;
    printf("Initialized CPU%d\n", link->cpuID);
}

//Function swaps two given integer pointers
void swap(int * a, int * b){
    int c = * a;
    * a = *b;
    * b = c;
}
int main(int argc, char ** argv){
    int h, q = 1;
    int numCPUs;
    printf("ENTER number of CPUs available:\n");
    //scanf("%d", &numCPUs);
    numCPUs = (argv[1][0] - 48); // CPUS manually set to 4
    for(h = 0; h < numCPUs; h++){
        initCPU(h+1);
    }
    printf("number of CPUs available entered: \n\t%d\n\nargv: %s\targc: %d\n\n", numCPUs, *argv, argc);
    
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
    printf("\n\tprinting initially:\n");
    printList();
    //struct node * currNode = head;
    //struct nodeCPU * currCPUNode = headCPU;
    int i, j, k = 0, t = 0;
    bool processList = (numCPUs > 0 && head != NULL);
    reverse(headCPU);
	printf("Time\tJob\n");
	struct node * curr = head;
	struct node * temp;
	struct nodeCPU * currCPU = headCPU;
	while(processList){	//list is not null and at least one processor available
		t++;
		if()
		while(k < numCPUs){
			
		}
	}
	/*while(processList && t < 25){
		curr = deleteFirst();
		currCPU = headCPU;
		if(curr == NULL){
			processList = false;
		}
		t++;
		if(curr->arrivalT <= t){
			temp = head;
			printf("%d\t",t);
			while(k < numCPUs && curr != NULL){
				j = curr->durationT;
				currCPU->currProcessID = curr->processID;
				printf("CPU[%d] has process: %c\t", currCPU->cpuID, currCPU->currProcessID);
				curr->durationT = j--;
				printf("j: %d\t", j);
				if(curr->durationT <= 0){
					printf("DO STUFF\n");//head = processFinished(head, t, temp);
				}
				/*for(i = 0; i < k; i++){
					head = head->next;
				}
				temp = curr;
				temp = temp->next;
				k++;
				//head = curr;
				//head = head->next;
				curr = curr->next;
				currCPU = currCPU->next;
			}
			head = curr;
			printf("\nWOW\n");
			printList();
			k = 0;
		}
		sort(t);
	}*/
}
