#include<prodcons.h>
extern volatile int n;
void producer(int count){
	
	int i;
	for(i=0;i<=count;i++){
		n=i;
		//printf("produced value= %d\n",n);		
	}	
}
