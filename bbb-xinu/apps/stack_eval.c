#include<xinu.h>
/*
unsigned int stack_size=0;
unsigned int iteration=20;
unsigned int stack_eval(){
	int base=0;	
	struct procent *prptr= &proctab[getpid()];
	kprintf("curent usage:%u",prptr->prstkbase-prptr->prstkptr);
	if(stack_size<(prptr->prstkbase-prptr->prstkptr)){
		stack_size=(prptr->prstkbase-prptr->prstkptr);
	}
	if(iteration==20){
		base=1;
	}
	if(iteration>1){
		iteration--;
		stack_eval();
	}
	if(base){
		kprintf("max stack usage:%u",stack_size);
	}
			
	return OK;
}
*/
