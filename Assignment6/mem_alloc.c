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
*/
// linked list for process holes and processes allocs. needs multiple points

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct memBlocks{
  int id;
  int base;
  int limit;
  int inUse;
  struct memBlocks * next;
};

struct memBlocks * baseMM;

int memT = -1;
char alg = 'a';
int numProcessesC = 0;
int numProcessesT = 0;
int mem_alloc_T = 0;
int mem_free_T = 0;

void printProc(struct memBlocks * temp){
    printf("\t%s, id: %d, inUse: %d, base: %d, limit: %d\n", ((temp->inUse == 0)? "Hole":"Process"), temp->id, temp->inUse, temp->base, temp->limit);
}

void printList(){
    struct memBlocks * temp = baseMM;
    printf("\n\tLIST<\n");
    while(temp != NULL){
        printProc(temp);
        temp = temp->next;
    }
    printf("\t>LIST\n\n");
}

void fail_alloc(struct memBlocks * proc){
    printf("Process %d failed to allocate %d memory\n", proc->id, proc->limit);
}

void fail_free(struct memBlocks * proc){
    printf("Process %d failed to free memory\n", proc->id);
}

int get_id(char line[]){
  const char delim[2] = " ";
  int j = strlen(line), i;
  char * lineTemp, num[j-1];
  lineTemp = strtok(line, delim);
  strncpy(num, &lineTemp[1], j-1);
  i = atoi(num);
  //printf("j: %d, line: %s, i: %d\n", j, line, i);
  return i;
}

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

struct memBlocks * proc_from_id(int id){
  struct memBlocks * temp = baseMM;
  while(temp->id != id && temp->next != NULL){
    temp = temp->next;
  }
  return temp;
}

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
//printf("\nDEFRAG<\n");
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
  //printList();
}

void process_out(struct memBlocks * proc){
    if(-1 != proc->id){
        mem_free_T += proc->limit;
        numProcessesT++;
      printf("Process %d leaving freeing base: %d - %d (base+limit)\n\n",proc->id,proc->base,proc->limit+proc->base);
      proc->inUse = 0;
      proc->id = -1;
      defrag();
    }
}

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

void firstFit(struct memBlocks * proc){
    struct memBlocks * temp = baseMM;
    if(proc->limit > memT){ // process is too large
        fail_alloc(proc);
        printf("\tBecause it is too large.\n\n");
        return;
    }
    while(temp != NULL){
      /*
        printf("baseMM->base: %d, baseMM->limit: %d\n", baseMM->base, memT);
        printProc(temp);
        printf("proc->id: %d, proc->base: %d, proc->limit: %d\n\n", proc->id, proc->base, proc->limit);
      */
        if(temp->inUse == 0 && temp->limit >= proc->limit){
            if(temp == baseMM){
                baseMM = proc;
            }
            split_hole(temp, proc);
            //printProc(proc);
            printf("Allocation successful! proc: %d\n\n", proc->id);
            return; // allocation successful
        }
        temp = temp->next;
    }
    fail_alloc(proc);
}

void bestFit(struct memBlocks * proc){
    struct memBlocks * temp = baseMM;
    struct memBlocks * bestBlock = baseMM;
    if(proc->limit > memT){ // process is too large
        fail_alloc(proc);
        printf("\tBecause it is too large.\n\n");
        return;
    }
    int residual = temp->limit;//999999999;
    while(temp != NULL){
        if(temp->inUse == 0){
            residual = temp->limit;
        }
        temp = temp->next;
    }    
    temp = baseMM;
    while(temp != NULL){
      /*
        printf("baseMM->base: %d, baseMM->limit: %d\n", baseMM->base, memT);
        printProc(temp);
        printf("proc->id: %d, proc->base: %d, proc->limit: %d\n\n", proc->id, proc->base, proc->limit);
      *//*
      printf("\n");
      printProc(proc);
      printf("\tproc VS temp residual: %d\n", residual);
      printProc(temp);
      printf("\n");*/
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
        //printProc(proc);
        printf("Allocation successful! proc: %d\n\n", proc->id);
        printList();
        return; // allocation successful
    }
    fail_alloc(proc);
}

void worstFit(struct memBlocks * proc){
    struct memBlocks * temp = baseMM;
    struct memBlocks * worstBlock = baseMM;
    if(proc->limit > memT){ // process is too large
        fail_alloc(proc);
        printf("\tBecause it is too large.\n\n");
        return;
    }
    int residual = temp->limit;//999999999;
    while(temp != NULL){
        if(temp->inUse == 0){
            residual = temp->limit;
        }
        temp = temp->next;
    }    
    temp = baseMM;
    while(temp != NULL){
      /*
        printf("baseMM->base: %d, baseMM->limit: %d\n", baseMM->base, memT);
        printProc(temp);
        printf("proc->id: %d, proc->base: %d, proc->limit: %d\n\n", proc->id, proc->base, proc->limit);
      */
      printf("\n");
      printProc(proc);
      printf("\tproc VS temp residual: %d\n", residual);
      printProc(temp);
      printf("\n");
      if(temp->inUse == 0 && temp->limit <= proc->limit){
        if(temp->limit - proc->limit >= residual){
          residual = temp->limit - proc->limit;
          worstBlock = temp;
        }
      }
        temp = temp->next;
    }
    if(worstBlock->inUse == 0){
        split_hole(worstBlock, proc);
        //printProc(proc);
        printf("Allocation successful! proc: %d\n\n", proc->id);
        printList();
        return; // allocation successful
    }
    fail_alloc(proc);
}

int main(int argc, char ** argv){
    if(argc != 4 && argc != 5){
        printf("Failure\n");
        return EXIT_FAILURE;
    }
    decipher(argv);
    printf("memT: %d, alg: %c\n", memT, alg);
    int i, j;
    char line[23], line2[23],*number;
    const char delim[2] = " ";
    int id, size;
    struct memBlocks * procIn1;
    baseMM = process_in(-1, memT); // initializing memory.
    j = scanf("%[^\n]", line);
    i = 0;
    while(j == 1){
      //printf("line: %s\n", line);
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

        //printf("Proc leaving: id: %d\n", (line[1]-48));
        procIn1 = proc_from_id(id);
        process_out(procIn1);
      }
      j = scanf("%s", line);
    }
      defrag();
      printf("\n\n\n");
      printList();
      printf("\n\tSUMMARY\n\n");
      printf("Total Processes created:\t%d\nTotal allocated memory:\t\t%d\nTotal Processes terminated:\t%d\nTotal freed memory:\t\t%d\n", numProcessesC, mem_alloc_T, numProcessesT, mem_free_T);
    return EXIT_SUCCESS;
}
