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
/*
void enQueue(struct memBlocks * newBlock){
	if(base == NULL && rear == NULL){ // queue is empty
		front = rear = newBlock;
    //strcpy(front->driver, carIn->driver);
    //strcpy(rear->driver, carIn->driver);
    base->next = NULL;
		return;
	}
	rear->next = newBlock;
  //strcpy(rear->next->driver, carIn->driver);
	rear = newBlock;
  //strcpy(rear->driver, carIn->driver);
  rear->next = NULL;
	return;
}*/

void decipher(char ** line){
    int i;
    for(i = 1; i < 5; i++){
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

struct memBlocks * process_in(int id, int limit){
    struct memBlocks * newProc = malloc(sizeof(struct memBlocks *));
    newProc->id = id;
    newProc->limit = limit;
}

struct memBlocks * process_out(struct memBlocks * proc){
    printf("Process %d leaving freeing base: %d - %d (base+limit)\n",proc->id,proc->base,proc->limit+proc->base);
    proc->inUse = 0;
    proc->id = -1;
}

void printProc(struct memBlocks * temp){
    printf("%s, temp->id: %d, temp->inUse: %d, temp->base: %d, temp->limit: %d\n", ((temp->inUse == 0)? "Hole":"Process"), temp->id, temp->inUse, temp->base, temp->limit);
}

void printList(){
    struct memBlocks * temp = baseMM;
    while(temp != NULL){
        printProc(temp);
        temp = temp->next;
    }
}

void fail_alloc(struct memBlocks * proc){
    printf("Process %d failed to allocate %d memory\n", proc->id, proc->limit);
}

void fail_free(struct memBlocks * proc){
    printf("Process %d failed to free memory\n", proc->id);
}

void split_hole(struct memBlocks * temp, struct memBlocks * proc){
    // base case temp == baseMM;
    if(temp->limit == memT){ // nothing points to temp
        //printf("\n\n\ntemp == baseMM\n");
        proc->base = temp->base;
        temp->base = temp->base + proc->limit;
        temp->limit = temp->limit - proc->limit;
        //proc = temp;
        proc->next = temp;
        proc->inUse = 1;
        /*printf("Hey there\n\n");
        //printProc(proc);
        printList();
        printf("Hey there\n\n");*/
        baseMM = proc;
    }
    else{
        struct memBlocks * temp_next = baseMM;
        while(temp_next->next != temp){
            temp_next = temp_next->next;
        }
        printProc(temp_next);
        proc->base = temp->base;
        temp->base = temp->base + proc->limit;
        temp->limit = temp->limit - proc->limit;
        proc->next = temp;
        proc->inUse = 1;
        if(temp_next == baseMM){
            temp_next = proc;
        }
        else{
            temp_next->next = proc;
        }
    }
    /*
    //temp_next->id = temp->id;
    temp_next->base = temp->base + proc->limit;
    temp_next->limit = temp->limit - temp_next->base;
        printf("Hey there\n");
        printProc(temp_next);
    //temp_next->inUse = temp->inUse;
    //temp_next->next = temp->next; //create new fragment
        //printProc(temp_next);
    temp->id = proc->id;
    //proc->base = temp->base;
    //temp->base = temp->base;
    temp->limit = proc->limit;
    temp->inUse = proc->inUse = 1;
        temp->next = temp_next;
        proc->next = temp_next;
        //printProc(temp_next);
    //temp->next = temp_next;
        //printProc(temp);
        //printProc(temp_next);*/
}

void firstFit(struct memBlocks * proc){
    int i;
    struct memBlocks * temp = baseMM;
    if(proc->limit > memT){ // process is too large
        fail_alloc(proc);
        printf("\tBecause it is too large.\n");
        return;
    }
    for(temp; temp != NULL; temp = temp->next){
        printf("baseMM->base: %d, baseMM->limit: %d\n", baseMM->base, memT);
        printProc(temp);
        printf("proc->id: %d, proc->base: %d, proc->limit: %d\n\n", proc->id, proc->base, proc->limit);
        if(temp->inUse == 0 && temp->limit >= proc->limit){
            if(temp == baseMM){
                baseMM = proc;
            }
            split_hole(temp, proc);
            printf("\tAllocation successful!\n");
            return; // allocation successful
        }
    }
    fail_alloc(proc);
}

int main(int argc, char ** argv){
    char *array1[] = {"mem_alloc", "-s", "300", "-f", "test1.txt"};
    char *array2[] = {"mem_alloc", "-s", "100000", "-b", "test1.txt"};
    char *array3[] = {"mem_alloc", "-s", "1000000", "-w", "test1.txt"};
    //printf("array[0]: %d\n", strlen(array1[1]));
    decipher(array1);
    printf("memT: %d, alg: %c\n", memT, alg);
    //command line input:
    /*if(argc != 4 && argc != 5){
        printf("Failure\n");
        return EXIT_FAILURE;
    }
    int memT = atoi(argv[2]);
    char alg = argv[3][1];*/
    int i;
    char lines[5][20];
    struct memBlocks * procIn1;
    struct memBlocks * procIn2;
    baseMM = process_in(-1, memT); // initializing memory.
    for(i = 0; i < 5; i++){
        procIn1 = process_in(i+1, 100);
        //procIn2 = process_in(i+5, 50);
        firstFit(procIn1);
        //firstFit(procIn2);
        //process_out(procIn1);
        if(i == 0){
           // process_out(procIn1);
        }
    }
    return EXIT_SUCCESS;
}