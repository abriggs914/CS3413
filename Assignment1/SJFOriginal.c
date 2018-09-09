#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct node{
   char * user;
   char processID;
   int arrivalT;
   int durationT;
   int stillProcessing;
   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//display the list
void printList(){
   struct node *ptr = head;
   printf("\n[ ");
	
   //start from the beginning
   while(ptr != NULL){
      printf("(%s, %c, %d, %d) ",ptr->user, ptr->processID, ptr->arrivalT, ptr->durationT);
      ptr = ptr->next;
   }
	
   printf(" ]\n");
}

//insert link at the first location
void insertFirst(char * userIn, char processIDIn, int arrivalIn, int durationIn){
   //create a link
   struct node * link = (struct node*) malloc(sizeof(struct node));
   link->user = malloc(20*sizeof(char));
   strcpy(link->user, userIn);
   link->processID = processIDIn;
   link->arrivalT = arrivalIn;
   link->durationT = durationIn;
   
   
    //printf("INSERTING(%s, %c, %d, %d\n) ",link->user, link->processID, link->arrivalT, link->durationT);
     
	
   //point it to old first node
   //link->next->user = head->user;
   link->next = head;
   //printf("LINK->NEXT(%s, %c, %d, %d)\n",head->user, head->processID, head->arrivalT, head->durationT);

   //point first to new first node
   head = link;
  // printf("HEAD(%s, %c, %d, %d)\n",head->user, head->processID, head->arrivalT, head->durationT);
 }

//delete first item
struct node* deleteFirst() {

   //save reference to first link
   struct node *tempLink = head;
	
   //mark next to first link as first 
   head = head->next;
	
   //return the deleted link
   return tempLink;
}

//is list empty
bool isEmpty(){
   return head == NULL;
}

int length(){
   int length = 0;
   struct node *current;
	
   for(current = head; current != NULL; current = current->next){
      length++;
   }
	
   return length;
}
/*
//find a link with given key
struct node* find(int key) {

   //start from the first link
   struct node* current = head;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->key != key) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}

//delete a link with given key
struct node* delete(int key){

   //start from the first link
   struct node* current = head;
   struct node* previous = NULL;
	
   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->key != key) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return current;
}*/

int userHasMoreProcessesWaiting(struct node ** head_ref){
   struct node * current = (struct node*) malloc(sizeof(struct node));
   current = * head_ref;
   struct node * next;
   int i, j, k;
   int size = length();
   k = size;
   for(i = 0 ; i < size - 1 ; i++, k--){
      current = head;
      next = head->next;
      for(j = 1 ; j < k ; j++){
         if(strcmp(current->user, next->durationT) == 0){
            current->stillProcessing++;
         }
      }
   }
}

void sort(int t){
   int i, j, k, tempArrival, tempDuration;
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
         }
			
         current = current->next;
         next = next->next;
      }
   }   
}

void reverse(struct node ** head_ref){
   struct node * prev   = NULL;
   struct node * current = * head_ref;
   struct node * next;
	
   while(current != NULL){
      next  = current->next;
      current->next = prev;   
      prev = current;
      current = next;
   }
	
   *head_ref = prev;
}

int processJob(struct node ** nodeIn, int t){
    struct node * currentlyExecuting = * nodeIn;
    if(currentlyExecuting->arrivalT <= t){
        currentlyExecuting->durationT--;
        if(currentlyExecuting->durationT == 0){
            currentlyExecuting->arrivalT = 9999999;
        }
        *nodeIn = currentlyExecuting;
        return 1;
    }
    return 0;
}

void main(){
   char header[25];
   char process = 'z';
   char space = 'z';
   int arrival = 0;
   int duration = 0;
   int counter = 0;
   int h = scanf("%s", header);
   while(h == 1){
        counter++;
        scanf("%c", &space); // reading tab
    	scanf("%c", &process);
    	scanf("%d", &arrival);
    	scanf("%d", &duration);
    	//printf("user: %s, processID: %c, arrivalT: %d, durationT: %d\n", header, process, arrival, duration);
        insertFirst(header, process, arrival, duration);
    	//printList();
    	h = scanf("%s", header);
   }
   printf("Original List: \n"); 
   printList();
   printf("\n");
   sort(0);
	
   printf("List after sorting the data: ");
   printList();
   
   int t = 0;
   int p = 1;
   int y;
   struct node * currNode = head;
   while(currNode->durationT != 0 || p != 0){
       y = processJob(&head, t);
       p=1;
       if(y == 1){
           printf("\n\ttime: %d\n", t);
           /*printf("\nprinting after processing\n");
           printList();*/
           if(head->durationT == 0 && head->stillProcessing == 0){
               printf("\n\nFINISHED PROCESS at t = %d: %c\n", t, head->processID);
               printf("IDLE\n");
               p = 0;
           }
           sort(t);
           printf("\nprinting after sort\n");
           printList();
       }
        t++;
        currNode = head;
   }
   printList();
   /*
   insertFirst("Bob", 'B', 2,20);
   insertFirst("Chris", 'C', 3,30);
   insertFirst("Dave", 'D', 4,1);
   insertFirst("Ed", 'E', 5,40);
   insertFirst("Fred", 'F', 6,56);*/
	
   //print list
/*
   while(!isEmpty()) {            
      struct node *temp = deleteFirst();
      printf("\nDeleted value:");
      printf("(%d,%d) ",temp->key,temp->data);
   }  
	
   printf("\nList after deleting all items: ");
   printList();
   insertFirst(1,10);
   insertFirst(2,20);
   insertFirst(3,30);
   insertFirst(4,1);
   insertFirst(5,40);
   insertFirst(6,56);
   
   printf("\nRestored List: ");
   printList();
   printf("\n");  

   struct node *foundLink = find(4);
	
   if(foundLink != NULL){
      printf("Element found: ");
      printf("(%d,%d) ",foundLink->key,foundLink->data);
      printf("\n");  
   } 
   else{
      printf("Element not found.");
   }

   delete(4);
   printf("List after deleting an item: ");
   printList();
   printf("\n");
   foundLink = find(4);
	
   if(foundLink != NULL){
      printf("Element found: ");
      printf("(%d,%d) ",foundLink->key,foundLink->data);
      printf("\n");
   } 
   else{
      printf("Element not found.");
   }
	*/
   printf("\n");
   sort(9999999);
	
   printf("List after sorting the data: ");
   printList();
	/*
   reverse(&head);
   printf("\nList after reversing the data: ");
   printList();*/
}
