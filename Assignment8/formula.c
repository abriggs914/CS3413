#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
	//code
	const int BAR = 36864, first = 65535;
	const long int offMask = 4095, pageMask = ((pow(2,32))-(pow(2,12))), val = pow(2,32);
	printf("offMask: %ld\npageMask: %ld\n", offMask, pageMask);
	long int num, count, temp,temp2;
	int scan = scanf("%ld", &num);
count = 0, temp = 0, temp2;
printf("count: %ld\n", count);
	while(count < pow(2,16)){
	//while(scan == 1 && count < 10){
			// printf("METHOD 1\n");
	    // printf("num: %ld & first: %ld -> (num&first = %d) -> - BAR(%ld) = %ld\n", num, first, (num&first), BAR, ((num&first)-BAR));
			// printf("\nMETHOD 2\n");
			// printf("num: %ld / 4*1024 %ld -> %ld, num & offMask -> %ld\n", num, (4*1024), (num/(4*1024)), (num&offMask));
			// scanf("%ld", &num);
			printf("r %ld\n", count);
			temp++;
			count+=(4096*2);
			// if(temp % 7 == 0){
	    // 	count+=40960;
			// 	temp == 0;
			// 	temp2++;
			// }
			// if(temp2 >= 5){
			// 	break;
			// }
	}
	return 0;
}
