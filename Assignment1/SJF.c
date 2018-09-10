#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node{
	char * user;
    char processID;
    int arrivalT;
    int durationT;
    int arrivalOrder;
    int lastOfUserProcessesFinishT;
    struct node *next;
};

struct node * head = NULL;
struct node * current = NULL;
int uniqueProcesses = 0;

//display the list
void printList(){
   struct node *ptr = head;
   printf("\n[ ");
	
   //start from the beginning
   while(ptr != NULL){
      printf("(%s, %c, AT%d, DT%d, AO%d, LP%d) ",ptr->user, ptr->processID, ptr->arrivalT, ptr->durationT, ptr->arrivalOrder, ptr->lastOfUserProcessesFinishT);
      ptr = ptr->next;
   }
	
   printf(" ]\n");
}

//insert link at the first location
void insertFirst(char * userIn, char processIDIn, int arrivalIn, int durationIn, int arrivalOrder){
	//create a link 
    struct node * link = (struct node*) malloc(sizeof(struct node));
    link->user = malloc(20*sizeof(char));
    strcpy(link->user, userIn);
    link->processID = processIDIn;
    link->arrivalT = arrivalIn;
    link->durationT = durationIn;
	link->arrivalOrder = arrivalOrder;
    link->next = head;
    head = link;
    //free(link->user);
    //free(link);
}

void freeList(){
    struct node* current = head;
    while(head != NULL){
    	current = current->next;
    	free(head->user);
    	free(head);
    	head = current;
    }
}

int length(){
    int length = 0;
    struct node * current;
	
    for(current = head; current != NULL; current = current->next){
		length++;
    }
    return length;
}

int checkIfUserInLine(char * userIn){
    struct node* current = head->next;  //start from the second link since head will be changed from sorting.
	//printf("COMPARING %s && %s\n", current->user, userIn);
    if(head == NULL){  //if list is empty
    	return 0;
    }
    while(strcmp(current->user, userIn) != 0){  //navigate through list
		//printf("COMPARING %s && %s\n", current->user, userIn);
        if(current->next == NULL){  //if it is last node
    	    return 0;
    	}
		else{  //go to next node
         current = current->next;
        }
    }
	if(current->durationT > 0){  //if data found
		return 1;
	}
    return 0;
}

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
        //printf("CURRENT(%s, %c, %d, %d/n) ",current->user, current->processID, current->arrivalT, current->durationT);
    
        for(j = 1 ; j < k ; j++){   
        	if(current->arrivalT <= t && next->arrivalT <= t){
            	if(current->durationT > next->durationT){
                 
                	tempUser = current->user;
                	current->user = next->user;
                	next->user = tempUser; 
                
		            tempProcess = current->processID;
		            current->processID = next->processID;
		            next->processID = tempProcess; 
		            
		            tempArrival = current->arrivalT;
		            current->arrivalT = next->arrivalT;
		            next->arrivalT = tempArrival;
		
		            tempDuration = current->durationT;
		            current->durationT = next->durationT;
		            next->durationT = tempDuration;

					tempArrOrd = current->arrivalOrder;
		            current->arrivalOrder = next->arrivalOrder;
		            next->arrivalOrder = tempArrOrd;
				}
			}
         	else if(current->arrivalT > next->arrivalT){
              
		        tempUser = current->user;
		        tempUser = current->user;
		        current->user = next->user;
		        next->user = tempUser; 
		        
		        tempProcess = current->processID;
		        current->processID = next->processID;
		        next->processID = tempProcess; 
		        
		        tempArrival = current->arrivalT;
		        current->arrivalT = next->arrivalT;
		        next->arrivalT = tempArrival;

		        tempDuration = current->durationT;
		        current->durationT = next->durationT;
		        next->durationT = tempDuration;

				tempArrOrd = current->arrivalOrder;
		        current->arrivalOrder = next->arrivalOrder;
		        next->arrivalOrder = tempArrOrd;
		        
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
	printf("HEY THERE\n");
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
        //printf("CURRENT(%s, %c, %d, %d)\n",current->user, current->processID, current->arrivalT, current->durationT);
        //printf("\tVS\nNEXT(%s, %c, %d, %d)\n",next->user, next->processID, next->arrivalT, next->durationT);
    
        for(j = 1 ; j < k; j++){   
        	if(current->arrivalOrder > next->arrivalOrder){
                	tempUser = current->user;
                	current->user = next->user;
                	next->user = tempUser; 
                
		            tempProcess = current->processID;
		            current->processID = next->processID;
		            next->processID = tempProcess; 
		            
		            tempArrival = current->arrivalT;
		            current->arrivalT = next->arrivalT;
		            next->arrivalT = tempArrival;
		
		            tempDuration = current->durationT;
		            current->durationT = next->durationT;
		            next->durationT = tempDuration;

					tempArrOrd = current->arrivalOrder;
		            current->arrivalOrder = next->arrivalOrder;
		            next->arrivalOrder = tempArrOrd;
		            
		            tempLPT = current->lastOfUserProcessesFinishT;
		        	current->lastOfUserProcessesFinishT = next->lastOfUserProcessesFinishT;
		        	next->lastOfUserProcessesFinishT = tempLPT;
			}

        	current = current->next;
        	next = next->next;
		}	
        
	}   
}
struct node * checkOtherProcess(char * userIn, char processIn){
	//start from the first link
    struct node* current = head->next;
	//printf("COMPARING %s && %s\n", current->user, userIn);

    //if list is empty
    if(head == NULL){
    	return NULL;
    }

    //navigate through list
    while(strcmp(current->user, userIn) != 0 || current->processID == processIn){
		//printf("COMPARING %s && %s\n", current->user, userIn);
	
    	//if it is last node
        if(current->next == NULL){
    	    return NULL;
    	}
		else{
         //go to next node

         current = current->next;

        }

    }/*
	printf("HEY\n");
	while(strcmp(current->user, userIn) != 0 && current->processID != processIn){
		current = current->next;
	}  */    

	
    //if data found
		return current;
}

struct node * getUserFromArrivalOrder(int n){//, int ** arr){
	struct node * temp = head;
	struct node * temp2 = head;
	//printf("LOOKING @ 1st: n: (%d), %s[%c]\n",n, temp->user, temp->processID);
	while(temp->arrivalOrder != (n+1)){
		//printf("\tLOOKING @ 2nd: %s[%c]\n", temp->user, temp->processID);
		temp2 = checkOtherProcess(temp->user, temp->processID);
		
		if(temp2 != NULL){
			return temp;
			//temp = temp2;
			//printf("\tLOOKING @ 3rd: %s[%c]\n", temp2->user, temp2->processID);
		}
		else{
			temp = temp->next;
		}
	}
	return temp;
}

char * getUserFromProcessID(char processIn){
	struct node * link = head;
	while(head != NULL){
		if(head->processID == processIn){
			return head->user;
		}
		head = head->next;
	}
	return 0;
}

int processJob(struct node ** nodeIn, int t){
	struct node * currentlyExecuting = * nodeIn;
    if(currentlyExecuting->arrivalT <= t){
		if(currentlyExecuting->durationT > 0){
		    currentlyExecuting->durationT--;
		    if(currentlyExecuting->durationT == 0){
		        currentlyExecuting->arrivalT = 9999999;
		    	currentlyExecuting->lastOfUserProcessesFinishT = t+1;
		    	//updateAllProcessesFromCurrUser(currentlyExecuting);
		    }
		    *nodeIn = currentlyExecuting;
		    return 1;
		}
    }
    return 0;
}

int alreadyCovered(struct node * ptrIn, int n){
	struct node* current = head;
	//printf("COMPARING %s && %s\n", current->user, userIn);
    if(head == NULL){  //if list is empty
    	return 0;
    }

	for(int i = 0; current != NULL; i++){
		if(strcmp(current->user, ptrIn->user) == 0){
			if(current->arrivalOrder < n){
				return 1; //already covered
			}
		}
		current = current->next;
	}
	return 0;
}

void swap(int *a, int * b){
    int c = *a;
	//printf("\t1\nc: %d, a: %d, b: %d\n", c, *a, *b);
    *a = *b;
	//printf("\t2\nc: %d, a: %d, b: %d\n", c, *a, *b);
    *b = c;
	//printf("\t3\nc: %d, a: %d, b: %d\n", c, *a, *b);
}

void main(){
	char title[100];
    char header[25];
    char process = 'z';
    char space = 'z';
    int arrival = 0;
    int duration = 0;
    int counter = 0;
int q = 1;
    scanf("%[^\n]", title);
	printf("Reading from file:\t%s\n\n", title);
    int h = scanf("%s", header);
	//printf("headerIn:%s\n\n", header);
    while(h == 1){
    	counter++;
        scanf("%c", &space); // reading tab
    	scanf("%c", &process);
    	scanf("%d", &arrival);
    	scanf("%d", &duration);
    	printf("user: %s, processID: %c, arrivalT: %d, durationT: %d\n", header, process, arrival, duration);
        insertFirst(header, process, arrival, duration, q);
    	//printList();
		q++;
    	h = scanf("%s", header);
    }
    sort(0);
   
    int t = 0;
    int p = 1;
    int y;
	int arr[q][2];
	int arrComp = 0;
	printf("\n\tbefore processing\n");
	printList();
    struct node * currNode = head;
    struct node * individualFinishes;
	printf("Time\tJob\n");
    while(currNode->durationT > -1 && p != 0 ){
    	y = processJob(&head, t);
     		printf("\t\tlastOfUserProcessesFinishT: %d\n",head->lastOfUserProcessesFinishT);
        p=1;
        if(y == 1){
     		printf("%d\t%c\n", t, head->processID);
            /*printf("\nprinting after processing\n");
            printList();*/
            if(head->durationT <= 0 && checkIfUserInLine(head->user) == 0){ // 0 == not in line, 1 == has a process in line
            	printf("\n\nFINISHED PROCESS at t = %d: %c\n",(t+1), head->processID);
                p = 0;
				arr[arrComp][0] = head->processID;
				arr[arrComp][1] = (t+1);
				arrComp++;
            }
            sort(t+1);
            //printf("\nprinting after sort\n");
            //printList();
        }
        t++;
        currNode = head;
		if(currNode->durationT > 0){
			p = 1;
		}
    }
    printf("%d\tIDLE\n", t);
	printf("\n\tafter processing\n");
	printList();
	printf("\nSummary:\n");
	int temp = 0;
	int temp2;
	for(int u = 0; u < arrComp; u++){
		for(int v = 0; v < 2; v++){
			printf("arr[%d][%d]: %d\n", u, v, arr[u][v]);
		}
	}/*
	while(uniqueProcesses < q-1){
		struct node * curr = getUserFromArrivalOrder(uniqueProcesses);//, arr);
		if(alreadyCovered(curr, uniqueProcesses) == 0){
			//printf("LOOKING @ (Main): %s, %c\n", curr->user, curr->processID);
			/*for(int j = 0; j < q-1; j++){
				//printf("COMPARING: ARR%d VS CURR%d\n", arr[j][0], curr->processID);
				if(arr[j][0] == curr->processID){
					temp2 = arr[j][1];
					break;
				}
			}
			printf("\t%s\t%d\n", curr->user, curr->lastOfUserProcessesFinishT); //temp2
			uniqueProcesses++;
			curr = curr->next;
			if(curr == NULL){ // end of list
				break;
			}
		}
		else{
			uniqueProcesses++;
		}
	}*/
    int size = length();
    int k = size;
	sortByArrival();
	printf("\n\tafter arrival sorting\n");
	printList();
	printf("\n\n\n");
	struct node * curr = head;
	int i, j, u, v, target, target2, target3, target4, tempChar, tempInt;
	int ** arrPtr = malloc(arrComp*sizeof(int *));
	for(i = 0; i < arrComp; i++){
		arrPtr[i] = malloc(2*sizeof(int));
	}
	for(i = 0; i < arrComp; i++){
		for(j = 0; j < 2; j++){
			arrPtr[i][j] = arr[i][j];
		}
	}
	printf("\ngot through the sticky stuff\nchecking anyway\n");
	for(int u = 0; u < arrComp; u++){
		for(int v = 0; v < 2; v++){
			printf("arrPtr[%d][%d]: %d\n", u, v, arrPtr[u][v]);
		}
	}
	for(i = 0; i < arrComp-1; i++){
		target = i;
		target2 = arr[i][1];
		target3 = arr[i][0];
		target4 = arr[i][1];
		for(j = i+1; j < arrComp; j++){
			printf("arrComp: %d, target: %d, arr[%d][0]: %d\n", arrComp, target, j, arr[j][0]);
			if(arr[target][0] > arr[j][0]){
				printf("arr[(target)%d][0] %d > arr[(j)%d][0] %d\n", target, arr[target][0], j, arr[j][0]);
				target = j;
			}
			//printf("B4target: %d, arr[%d][0]: %d\n", target, j, arr[j][0]);
				swap(&arr[target][0], &arr[i][0]);
				//printf("AFtarget: %d, arr[%d][0]: %d\n\n", target, j, arr[j][0]);
				//printf("B4target2: %d, arr[%d][1]: %d\n", target2, j, arr[j][0]);
				swap(&arr[target][1], &arr[i][1]);
				//printf("AFtarget2: %d, arr[%d][1]: %d\n", target2, j, arr[j][0]);
				/*printf("\npreparing to swap\n");
				tempChar = arr[target][0];
				tempInt = arr[target][1];
				arr[target][0] = arr[j][0];
				arr[target][1] = arr[j][1];
				arr[j][0] = tempChar;
				arr[j][0] = tempInt;*/
				swap(&target3 ,&arrPtr[j][0]);
				swap(&target4 ,&arrPtr[j][1]);
		}
	}
	printf("\n\t2\ngot through the sticky stuff\nchecking anyway\n");
	for(int u = 0; u < arrComp; u++){
		for(int v = 0; v < 2; v++){
			printf("arr[%d][%d]: %d\n", u, v, arr[u][v]);
		}
	}
	
	printf("\nchecking again\n");
	for(int u = 0; u < arrComp; u++){
		for(int v = 0; v < 2; v++){
			printf("arrPtr[%d][%d]: %d\n", u, v, arrPtr[u][v]);
		}
	}
	/*
	for(i = 0; i < arrComp-1; i++){
		target = arrPtr[i][0];
		for(j = i+1; j < arrComp; j++){
			printf("arrComp: %d, target: %d, arrPtr[%d][0]: %d\n", arrComp, target, j, arrPtr[j][0]);
			if(target > arrPtr[j][0]){
				printf("\npreparing to swap\n");
				tempChar = arrPtr[target][0];
				tempInt = arrPtr[target][1];
				arrPtr[target][0] = arrPtr[j][0];
				arrPtr[target][1] = arrPtr[j][1];
				arrPtr[j][0] = tempChar;
				arrPtr[j][0] = tempInt;
			}
		}
	}*/
	//arrangeByProcessID(arrPtr, arrComp);
	/*
	for(v = 0; v < arrComp-1; v++){		//loops for number of process arrivals detected
		for(u = 0; u < q; u++){		//loops for number of unique users detected
			printf("COMPARING%s's : %c, VS arr[%d][0]: %d, arr[%d][1]: %d, owned by %s\n", curr->user, curr->processID, u, arr[u][0], u, arr[u][1], getUserFromProcessID(arr[u][0]));
			if(arr[u][0] == curr->processID){
				//for(v = 0; v < 2; v +=2){
					printf("%s\t%d", getUserFromProcessID(arr[u][0]), arr[u][1]);
				//}
				printf("\n");
				i++;
				break;
			}
		}
		curr = curr->next;
	}*/
	//printf("\t%s\t%d\n", curr->user, curr->lastOfUserProcessesFinishT); //temp2
	freeList();
	printf("\n\tafter freeing\n");
	printList();
}
