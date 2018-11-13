/*
* CS3413 Assignment 6
* Oct.31/18
* Avery Briggs
* 3471065
*
* C program to simulate memory
* allocations for processes.
* The program uses one of three
* algorithms for allocating memory.
* First fit, Best fit, Worst fit.
* command line format:
* ./mem_alloc -s # [-f,-b,-w] (< file.txt)
* *one of the three algorithms.
* file input is optional.
*/
// linked list for process holes and processes allocs. needs multiple points

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//memBlocks struct stores the data associated with a process or a hole
//id    - process id
//base  - the location in memory the beginning of the process was assigned
//limit - the amount of memory the process is holding in memory
//inUse - 0 - not in use, 1 - in use
//next  - points to the next process in memory
struct memBlocks{
  int id;
  int base;
  int limit;
  int inUse;
  struct memBlocks * next;
};

//The base of the block of memory to be allocated.
struct memBlocks * baseMM;

int memT = -1; // total memory in the system
char alg = 'a'; // char representation of the given algorithm
int numProcessesC = 0; // running total for created processes
int numProcessesT = 0; // running total for terminated processes
int mem_alloc_T = 0; // running total for the amount of memory allocated
int mem_free_T = 0; // running total for the amount of memory freed

//Function is called to report an allocation error
void fail_alloc(struct memBlocks * proc){
    printf("Process %d failed to allocate %d memory\n\n", proc->id, proc->limit);
}

//Function is called to report freeing error
void fail_free(struct memBlocks * proc){
    printf("Process %d failed to free memory\n\n", proc->id);
}

//Function returns the process id from the stdin input line.
int get_id(char line[]){
  const char delim[2] = " ";
  int j = strlen(strtok(line, delim));
  char * num = malloc((j-1)*sizeof(char));
  strncpy(num, &line[1], j-1);
  int res = atoi(num);
  free(num);
  return res;
}

//Function is called to pull the memT and alg values from argv
void decipher(char ** line){
    int i;
    for(i = 1; i < 4; i++){
        if(strlen(line[i]) == 2){
            if(line[i][1] == 's'){
                memT = atoi(line[i+1]);
            }
            else if(line[i][1] == 'f' || line[i][1] == 'b' || line[i][1] == 'w'){
                alg = line[i][1];
            }
        }
    }
}

//Function returns the process corresponding to the given id from memory list
struct memBlocks * proc_from_id(int id){
  struct memBlocks * temp = baseMM;
  while(temp->id != id && temp->next != NULL){
    temp = temp->next;
  }
  return temp;
}

//Function is called to create a new space in heap for a processes
//parameters:
//id    - the new processes id
//limit - the new processes required memory
struct memBlocks * process_in(int id, int limit){
    struct memBlocks * newProc = malloc(sizeof(struct memBlocks *));
    newProc->id = id;
    newProc->limit = limit;
    if(numProcessesC > 0){
      newProc->inUse = 1;
    }
    return newProc;
}

//Function is called to concatenate and free adjacent empty spaces.
//Only frees gaps in memory that are not in use, and adjacent to each other.
void defrag(){
  struct memBlocks * temp = baseMM;
  struct memBlocks * temp_next;
  if(temp != NULL){
    temp_next = temp->next;
    while(temp_next != NULL){
      //printProc(temp);
      if(temp->inUse == 0 && temp_next->inUse == 0){
        temp->limit += temp_next->limit;
        temp->next = temp_next->next;
        temp_next->base = temp->base;
        free(temp_next);
        return;
      }
      temp = temp_next;
      temp_next = temp->next;
    }
  }
}

//Function prints the attributes of teh list using a template
void printProc(struct memBlocks * temp){
    printf("\t%s, id: %d, inUse: %s, base: %d, limit: %d\n", ((temp->inUse == 0)? "Hole":"Process"), temp->id, ((temp->inUse==0)?"false":"true"), temp->base, temp->limit);
}

//Function prints the contents of the memory list using a template
void printList(){
    struct memBlocks * temp = baseMM;
    defrag();
    printf("\n\tMEM_LIST<\n");
    while(temp != NULL){
        printProc(temp);
        temp = temp->next;
    }
    printf("\t>MEM_LIST\n\n");
}

//Function is called to free the allocated memory for the given process.
//Calls defrag() to help squish the memory list and adjacent holes.
void process_out(struct memBlocks * proc){
    if(-1 != proc->id){
        mem_free_T += proc->limit;
        numProcessesT++;
      printf("Freeing process %d, base: %d to %d (base+limit)\n\n",proc->id,proc->base,proc->limit+proc->base);
      proc->inUse = 0;
      proc->id = -1;
      defrag();
    }
}

//Function is called to take a given hole in memory, and split it's space
//with the given processes. Adjusts baseMM accordingly if the space is
//located at the beginning of memory.
void split_hole(struct memBlocks * temp, struct memBlocks * proc){
    //printf("\tsplit_hole\n");
    if(temp->limit == memT || temp->base == 0){ // nothing points to temp
        //printf("\n\n\ntemp == baseMM\n");
        proc->base = temp->base;
        temp->base += proc->limit;
        temp->limit -= proc->limit;
        proc->next = temp;
        proc->inUse = 1;
        baseMM = proc;
    }
    else{
        struct memBlocks * temp_next = baseMM;
        while(temp_next->next != temp){
            temp_next = temp_next->next;
        }
        proc->base = temp->base;
        temp->base += proc->limit;
        temp->limit -= proc->limit;
        proc->next = temp;
        proc->inUse = 1;
        temp_next->next = proc;
    }
    numProcessesC++;
    mem_alloc_T += proc->limit;
}

//Function uses a FCFS algorithm to distribute memory blocks to processes.
//Ensures that the memory allocated at one time does not exceed the Total
//available memory.
void firstFit(struct memBlocks * proc){
    struct memBlocks * temp = baseMM;
    if(proc->limit > memT){ // process is too large
        fail_alloc(proc);
        return;
    }
    while(temp != NULL){
        if(temp->inUse == 0 && temp->limit >= proc->limit){
            if(temp == baseMM){
                baseMM = proc;
            }
            split_hole(temp, proc);
            printf("allocation successful! proc: %d\n", proc->id);
            printList();
            return; // allocation successful
        }
        temp = temp->next;
    }
    fail_alloc(proc);
}

//Function computes the residual space for each process, if it were to
//be given that block. It decides which block will produce the smallest
//residual, and allocates that block's memory.
void bestFit(struct memBlocks * proc){
    struct memBlocks * temp = baseMM;
    struct memBlocks * bestBlock = baseMM;
    if(proc->limit > memT){ // process is too large
        fail_alloc(proc);
        return;
    }
    int residual = temp->limit;
    while(temp != NULL){
        if(temp->inUse == 0){
            residual = temp->limit;
        }
        temp = temp->next;
    }
    temp = baseMM;
    while(temp != NULL){
      if(temp->inUse == 0 && temp->limit >= proc->limit){
        if(temp->limit - proc->limit < residual){
          residual = temp->limit - proc->limit;
          bestBlock = temp;
        }
      }
        temp = temp->next;
    }
    if(bestBlock->inUse == 0){
        split_hole(bestBlock, proc);
        printf("allocation successful! proc: %d\n", proc->id);
        printList();
        return; // allocation successful
    }
    fail_alloc(proc);
}

//Function computes the residual space for each process, if it were to
//be given that block. It decides which block will produce the largest
//residual, and allocates that block's memory.
void worstFit(struct memBlocks * proc){
    struct memBlocks * temp = baseMM;
    struct memBlocks * worstBlock = baseMM;
    if(proc->limit > memT){ // process is too large
        fail_alloc(proc);
        return;
    }
    int residual = temp->limit;
    while(temp != NULL){
        if(temp->inUse == 0 && temp->limit > proc->limit){
            residual = temp->limit - proc->limit;
            worstBlock = temp;
        }
        temp = temp->next;
    }
    temp = baseMM;
    while(temp != NULL){
      if(temp->inUse == 0 && temp->limit >= proc->limit){
        if(temp->limit - proc->limit > residual){
          residual = temp->limit - proc->limit;
          worstBlock = temp;
        }
      }
        temp = temp->next;
    }
    if(worstBlock->inUse == 0){
        split_hole(worstBlock, proc);
        printf("allocation successful! proc: %d\n", proc->id);
        printList();
        return; // allocation successful
    }
    fail_alloc(proc);
}

//Function is called at the end of input, to determine which Processes
//have not freed their allocated memory yet.
void left_over(){
  struct memBlocks * temp = baseMM;
  int stillRunning = 0;
  while(temp != NULL){
    if(temp->id > 0){
      stillRunning++;
      fail_free(temp);
    }
    temp = temp->next;
  }
  if(stillRunning == 0){
    printf("\tNONE\n");
  }
}

int main(int argc, char ** argv){
    if(argc != 4 && argc != 5){
        printf("Failure\nUsage: ./mem_alloc -s # -(one of; -f, -b, -w) #\n");
        return EXIT_FAILURE;
    }
    decipher(argv);
    if(memT < 0 || (alg != 'f' && alg != 'b' && alg != 'w')){
      printf("Failure\nUsage: ./mem_alloc -s # -(one of; -f, -b, -w) #\n");
      return EXIT_FAILURE;
    }
    printf("Total memory available: %d, algorithm: %c\nYou may enter 'q' anytime to stop input.\n\n", memT, alg);
    int i, j;
    char line[23], line2[23],*number;
    const char delim[2] = " ";
    const char endIn[2] = "q";
    int id, size;
    struct memBlocks * procIn1;
    baseMM = process_in(-1, memT); // initializing memory.
    j = scanf("%[^\n]", line);
    i = 0;
    while(j == 1){
      //printf("line: %s\n", line);
      if(strcmp(line, endIn)==0){
        break;
      }
      strcpy(line2, line);
      id = get_id(line2);
      if(line[0] == 'N'){
        if(i == 0){
          number = strtok(line, delim);
          number = strtok(NULL, delim);
          size = atoi(number);
          i++;
        }
        else{
          scanf("%s", number);
          size = atoi(number);
        }
        //printf("New Proc: id: %d, size: %d\n", id, size);
        procIn1 = process_in(id, size);
        if(alg == 'f'){
          firstFit(procIn1);
        }
        else if(alg == 'b'){
          bestFit(procIn1);
        }
        else{
          worstFit(procIn1);
        }
      }
      else{
        procIn1 = proc_from_id(id);
        process_out(procIn1);
      }
      j = scanf("%s", line);
    }
      defrag();
      printf("\tProcesses still running:\n");
      left_over();
      printf("\n");
      printList();
      printf("\n\tSUMMARY\n");
      printf("Total Processes created:\t%d\nTotal allocated memory:\t\t%d\nTotal Processes terminated:\t%d\nTotal freed memory:\t\t%d\n", numProcessesC, mem_alloc_T, numProcessesT, mem_free_T);
      free(baseMM);
    return EXIT_SUCCESS;
}
