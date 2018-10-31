#include <stdio.h>

int main() {
	//code
	const int BAR = 36864, first = 65535;
	long int num;
	int scan = scanf("%ld", &num), count = 0;
	while(scan == 1 && count < 10){
	    printf("num: %ld & first: %ld -> (num&first = %d) -> - BAR(%ld) = %ld\n", num, first, (num&first), BAR, ((num&first)-BAR));
	    scanf("%ld", &num);
	    count++;
	}
	return 0;
}