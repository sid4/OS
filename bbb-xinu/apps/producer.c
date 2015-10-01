#include<prodcons.h>

void producer(int count){
	int i;
	n = 0;
	for(i=0;i<count;i++){
		wait(consumed);
		n++;
		printf("produced value= %d\n", n);		
		signal(produced);
	}	
}
