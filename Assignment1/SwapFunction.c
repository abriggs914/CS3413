#include <stdio.h>

void swap(int *, int *);
int main() {
	//code
	int a = 0, b = 0, *c, *d; 
	scanf("%d", &a);
	scanf("%d", &b);
	c = &a;
	d = &b;
	printf("a: %d, b: %d\n", a, b);
	printf("c: %d, d: %d\n", *c, *d);
	//swap(c,d);
	swap(&a,&b);
	printf("c: %d, d: %d\n", *c, *d);
	printf("a: %d, b: %d\n", a, b);
	return 0;
}

void swap(int *a, int * b){
    int c = *a;
	printf("\t1\nc: %d, a: %d, b: %d\n", c, *a, *b);
    *a = *b;
	printf("\t2\nc: %d, a: %d, b: %d\n", c, *a, *b);
    *b = c;
	printf("\t3\nc: %d, a: %d, b: %d\n", c, *a, *b);
}
