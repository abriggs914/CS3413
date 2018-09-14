/*
*	C program to read in a list of users,
*	processes, their arrival times, and
*	their expected runtime. Program follows
*	a SJF (shortest job first) algorithm for
*	selecting a process to pass to the CPU.
*	
*	CS3413 Assignment 1
*	Sept 2018
*	Avery Briggs
*	3471065
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
	Process struct
	user - user name
	processID - processName given
	arrivalT - time process arrived to queue
	durationT - expected process runtime
	arrivalOrder - individual order of process arrivals
	lastOfUserProcessesFinishT - Time at which last process owned by particular user finishes
	usersFirstProcess - The first process that arrived for a specific user
	next - points to the next process in line
*/
struct node{
	char * user;
    char processID;
    int arrivalT;
    int durationT;
    int arrivalOrder;
    int lastOfUserProcessesFinishT;
    char usersFirstProcess;
    struct node *next;
};

// start of the list
struct node * head = NULL;

char getUsersFirstProcess(char * userIn, char idIn){
	struct node * link = head;
	char target = idIn;
	while(link != NULL){
    	if(strcmp(link->user, userIn) == 0){
    		if(link->processID < target){
    			target = link->processID;
    		}
    	}
    	link = link->next;
    }
    return target;
}


//Function prints the contents of the list in a formatted text
void printList(){
   struct node * ptr = head;
   while(ptr != NULL){
      printf("\t%s, %c, AT%d, DT%d, AO%d, LP%d\n",ptr->user, ptr->processID, ptr->arrivalT, ptr->durationT, ptr->arrivalOrder, ptr->lastOfUserProcessesFinishT);
      ptr = ptr->next;
   }
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
	link->usersFirstProcess = getUsersFirstProcess(link->user, processIDIn);
    link->next = head;
    head = link;
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

/* 
	Function arranges the list of processes based on the SJF algorithm
    function takes in time t to determine whether a process is eligible
    to be processed, then sorts by shortest job.
*/
void sort(int t){
	int i, j, k, tempArrival, tempDuration, tempArrOrd, tempLPT;
 	char tempProcess;
    char * tempUser;
    struct node * current;
    struct node * next;	
    int size = length();
    k = size;	
    for(i = 0 ; i < size - 1 ; i++, k--){
    	current = head;
        next = head->next;    
        for(j = 1 ; j < k ; j++){   
        	if(current->arrivalT <= t && next->arrivalT <= t){
            	if(current->durationT > next->durationT){
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
			}
         	else if(current->arrivalT > next->arrivalT){
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
         	current = current->next;
        	next = next->next;
		}
	}   
}
void sortByArrival(){
	int i, j, k, tempArrival, tempDuration, tempArrOrd, tempLPT;
 	char tempProcess;
    char * tempUser;
    struct node * current;
    struct node * next;
    int size = length();
    k = size;
    for(i = 0 ; i < size - 1; i++, k--){
    	current = head;
        next = head->next;    
        for(j = 1 ; j < k; j++){   
        	if(current->arrivalOrder > next->arrivalOrder){
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

//Function simulates a CPU cycle, decreasing the duration left on
//a current process by 1.
int processJob(struct node ** nodeIn, int t){
	struct node * currentlyExecuting = * nodeIn;
    if(currentlyExecuting->arrivalT <= t){
		if(currentlyExecuting->durationT > 0){
		    currentlyExecuting->durationT--;
		    if(currentlyExecuting->durationT == 0){
		        currentlyExecuting->arrivalT = 9999999;  //using this as a check for when process is finished.
		    	currentlyExecuting->lastOfUserProcessesFinishT = t+1;
		    }
		    *nodeIn = currentlyExecuting;
		    return 1;
		}
    }
    return 0;
}

//Function swaps two given integer pointers
void swap(int * a, int * b){
    int c = * a;
    * a = *b;
    * b = c;
}

int main(){
	char title[100];
    char userIn[25];
    char process;
    char space;
    int arrival;
    int duration;
	int q = 1, h;
    scanf("%[^\n]", title);
    h = scanf("%s", userIn);
    while(h == 1){
        scanf("%c", &space); // reading tab
    	scanf("%c", &process);
    	scanf("%d", &arrival);
    	scanf("%d", &duration);
        insertFirst(userIn, process, arrival, duration, q);
		q++;
    	h = scanf("%s", userIn);
    }
    sort(0);
   
    int t = 0, p = 1, y, arrComp = 0;
	int arr[q][2];
    struct node * currNode = head;
	printf("Time\tJob\n");
    while(currNode->durationT > -1 && p != 0 ){
    	y = processJob(&head, t);
        p=1;
        if(y == 1){
     		printf("%d\t%c\n", t, head->processID);
            if(head->durationT <= 0 && checkIfUserInLine(head->user) == 0){ // 0 == not in line, 1 == has a process in line
                p = 0;
				arr[arrComp][0] = head->processID;
				arr[arrComp][1] = (t+1);
				arrComp++;
            }
            sort(t+1);
        }
        t++;
        currNode = head;
		if(currNode->durationT > 0){
			p = 1;
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
			arrPtr[i][j] = arr[i][j];
		}
	}
	for(i = 0; i < arrComp-1; i++){
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
	free(arrPtr);
	freeList();
	return 0;
}
