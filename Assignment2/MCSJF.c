/*
*	C program to read in a list of users,
*	processes, their arrival times, and
*	their expected runtime. Program follows
*	a SJF (shortest job first) algorithm for
*	selecting a process to pass to the CPU.
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

void printFinishedList(){
   struct node * ptr = finishedProcesses;
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

// Function is called to free the list in heap
void freeList(){
    struct node * current = head;
    while(head != NULL){
    	current = current->next;
    	free(head->user);
    	free(head);
    	head = current;
    }
}

void freeCPUList(){
    struct nodeCPU * current = headCPU;
    while(head != NULL){
    	current = current->next;
    	free(headCPU);
    	headCPU = current;
    }
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

// Function determines whether a specific user still has at least one process waiting for the CPU 
// returns 0 if user is not waiting, else 1
int checkIfUserInLine(char * userIn){
    struct node * current = head->next;  //start from the second link since head will be changed from sorting.
    if(head == NULL){  //if list is empty
    	return 0;
    }
    while(strcmp(current->user, userIn) != 0){
        if(current->next == NULL){  //last node
    	    return 0;
    	}
		else{  //go to next node
         current = current->next;
        }
    }
	if(current->durationT > 0){  //if user is waiting
		return 1;
	}
    return 0;
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
        	if(current->arrivalT <= t && next->arrivalT <= t){
            	if(current->durationT > next->durationT){
        			swapProcesses(current, next);
        		}
			}
         	else if(((current->arrivalT > next->arrivalT) && next->processed == 0)){
        		swapProcesses(current, next);
			}
         	current = current->next;
        	next = next->next;
		}
	}   
}

void deleteFirst(){
   struct node * link = head;
   head = head->next;
   free(link); 
}

void sortByArrival(){
	int i, j, k;//, tempArrival, tempDuration, tempArrOrd, tempLPT, tempPCD;
 	//char tempProcess;
    //char * tempUser;
    struct node * current;
    struct node * next;
    int size = length();
    k = size;
    for(i = 0 ; i < size - 1; i++, k--){
    	current = head;
        next = head->next;    
        for(j = 1 ; j < k; j++){   
        	if(current->arrivalOrder > next->arrivalOrder){
        		swapProcesses(current, next);
			}
        	current = current->next;
        	next = next->next;
		} 
	}   
}

//Function returns another process (if any) to be checked in sortByArrival()
struct node * checkOtherProcess(char * userIn, char processIn){
    struct node* current = head->next;
    if(head == NULL){  //if list is empty
    	return NULL;
    }
    while(strcmp(current->user, userIn) != 0 || current->processID == processIn){	
        if(current->next == NULL){ //last node
    	    return NULL;
    	}
		else{  //go to next node
         current = current->next;
        }
    }
	return current;  //if data found
}

//Function takes in a number corresponding to an arrival time for a process.
//function returns the process node from the arrival time
struct node * getUserFromArrivalOrder(int n){
	struct node * temp = head;
	struct node * temp2 = head;
	while(temp->arrivalOrder != (n+1)){
		temp2 = checkOtherProcess(temp->user, temp->processID);
		
		if(temp2 != NULL){
			return temp;
		}
		else{
			temp = temp->next;
		}
	}
	return temp;
}

//Function returns the user name for a unique processID
char * getUserFromProcessID(char processIn){
	struct node * link = head;
	while(link != NULL){
		if(link->processID == processIn){
			return link->user;
		}
		link = link->next;
	}
	return 0;
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
    struct node * currNode = head;
    struct nodeCPU * currCPUNode = headCPU;
	printf("Time\tJob\n");
	int i, j, k, t = 0;
	i = 100;
	j = -110;
	i = i-j;
	while(head != NULL && currNode->durationT > 0){
		//printProcessNode(currNode);
		processJob(head, headCPU, t+1, numCPUs, 0);  //redefine processJob with structs.
		printf("\n");
		/*while(k < numCPUs){
			if(currNode->next != NULL){
				currNode = currNode->next;
				currCPUNode = currCPUNode->next;
				processJob(currNode, currCPUNode, t+1, numCPUs, 0);
				printf("\n");
			}
			k++;
		}*/
		sort(t+1);
		currNode = head;
		if(head->processed == 1){
			deleteFirst();
			currNode = head;
		}
		printf("\n\t\tgot here in main\n");
		printList();
		printf("\t\tmain\n");
		t++;
		//headCPU = headCPU;
	}
	printf("\nAfter processing loop\n\n");
	printList();
	printf("\n\n\n");
	printFinishedList();
   /*
    int t = 0, p = 1, y, arrComp = 0, w;
	int arr[q][2];
    struct node * currNode = head;
	printf("Time\tJob\n");
	if(currNode != NULL){
		for(w = 0; w < numCPUs; w++){
				while(currNode->durationT > -1 && p != 0){ //simulate processing, t++ and duration--
					y = processJob(&head, t);
					p=1;
					if(y == 1){
				 		printf("%d\t%c\n", t, head->processID);
						if(head->durationT <= 0 && checkIfUserInLine(head->user) == 0){ // 0 == not in line, 1 == has a process in line
						    p = 0;
							arr[arrComp][0] = head->processID;	//storing these values as a record of when a user finishes all their processes
							arr[arrComp][1] = (t+1);			//and when they were completed. using this array for creating the second table
							arrComp++;
						}
						sort(t+1);
						printf("\nPrinting after sort:\n");
						printList();
						printf("\n\n");
					}
					t++;
					currNode = head;
					if(currNode->durationT > 0){
						p = 1;		//using p as a helper check for when something is processed, setting to 1 to continue, else quit
					}
				}
			}
		}
	}
    
    printf("%d\tIDLE\n", t);
	printf("\nSummary:\n");
	int i, j, target;
	sortByArrival();
	int ** arrPtr = malloc(arrComp*sizeof(int *));
	for(i = 0; i < arrComp; i++){
		arrPtr[i] = malloc(2*sizeof(int));	
	}
	for(i = 0; i < arrComp; i++){
		for(j = 0; j < 2; j++){
			arrPtr[i][j] = arr[i][j];	//copying array to sort
		}
	}
	for(i = 0; i < arrComp-1; i++){	//selection sort algorithm to sort int array
		target = i;
		for(j = i+1; j < arrComp; j++){
			if(arrPtr[target][0] > arrPtr[j][0]){
				target = j;
			}

		}
		swap(&arrPtr[target][0] ,&arrPtr[i][0]);
		swap(&arrPtr[target][1] ,&arrPtr[i][1]);
	}
	for(i = 0; i < arrComp; i++){
		printf("%s\t%d\n", getUserFromProcessID(arrPtr[i][0]), arrPtr[i][1]);
	}
	for(i = 0; i < arrComp; i++){	
		free(arrPtr[i]);
	}
	printList();
	free(arrPtr);	//freeing array in heap
	freeList();		//freeing list pointers
	freeCPUList();	//freeing CPU list pointers*/
	return 0;
}
