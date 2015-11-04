#include<xinu.h>
#include <stdlib.h>

unsigned int iteration;
unsigned int stack_eval(){

	if(iteration>1){
		//kprintf("pid:%d",getpid());
		//getstk(400);
		
		iteration--;
		stack_eval();
	}		

	return OK;
}
shellcmd ex9_7 (int nargs , char *args[]){
	iteration=50;
	pid32 pid=create(stack_eval, 1024, 20, "stack_eval", 0);
	kprintf("\nprocess for stack eval:%d\n",pid);
	resume(pid);
	sleep(20);
	return 0;
}


