#include<prodcons.h>
#include<stdio.h>
extern int n;
void consumer(int count){	
	while(n<=count){
		printf("consumed value= %d\n",n);
	}

}

