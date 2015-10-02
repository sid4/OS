#include<prodcons.h>

void consumer(int count){

//printf("count %d/n",count);
//printf("Count = %d, n = %d", count, n);
	while(n<=count){
		wait(produced);
		printf("consumed value= %d\n",n);
		signal(consumed);
	}
semdelete(produced);
semdelete(consumed);
}

