#include<prodcons.h>
#include<stdio.h>

extern int n;
void producer(int count){
	int i;
	for(i=0;i<=count;i++){
		n=i;
		printf("produced value= %d\n",n);		
	}	
}
