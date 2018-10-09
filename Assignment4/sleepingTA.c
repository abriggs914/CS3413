#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct job{	
	int studentID; 
	int arrival;
	int duration;
	struct job * next;
};

int studentIDs = 0;
struct job * head;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int num = 0, studentNumber = 0;

void * foo(){
	pthread_mutex_lock(&mutex);
	printf("\npthread_ID: %lld, num: %d\n", (long long)pthread_self(), num);
	num++;
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

void insertFirst(int idIn){
	struct job * link = (struct job *) malloc(sizeof(struct job));
	link->studentID = idIn;
	printf("idIn: %d\n", idIn);
    link->next = NULL;
    head = link;
}

int main(int argc, char ** argv){
	int c, t, n, i, numStudents, temp;
	struct job * tjob;
	time_t tm; 
	srand((unsigned) time(&tm));
	if(argc != 7){
		printf("Usage: 'a.out -c 5 -t 200 -n 12'\n\tPlease try again.\n");
		return 0;
	}
	c = atoi(argv[2]);
	t = atoi(argv[4]);
	n = atoi(argv[6]);
	numStudents = rand() % n; //numStudents (student per (n) seconds)
	printf("Number of students for extra help: %d\n", numStudents);
	printf("\tc: %d\n\tt: %d\n\tn: %d\n\tnum: %d\n", c, t, n, rand() % n);pthread_t ta[4];//aid, bid, cid, did; // 4 TAs (a-d)id
	for(i = 0; i < 4; i++){
		pthread_create(&ta[i], NULL, foo, NULL);
	}
	temp = numStudents;
	while(temp > 0){
		pthread_mutex_lock(&mutex);
		insertFirst(++studentIDs);	
		pthread_mutex_unlock(&mutex);
		temp--;
	}
	
	for(i = 0; i < 4; i++){
		pthread_join(ta[i], NULL);
	}
	return EXIT_SUCCESS;
}
