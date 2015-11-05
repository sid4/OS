#include<xinu.h>
#include <stdlib.h>

unsigned int iteration;
unsigned int stack_eval(){

	if(iteration>1){
		iteration--;
		stack_eval();
	}		

	return OK;
}

shellcmd ex9_7 (int nargs , char *args[]){
	if(nargs==2){
	iteration=atoi(args[1]);
	}
	else{	
	iteration=100;
	}
	demo_pid=getpid();
	pid32 pid=create(stack_eval, 1024, 20, "stack_eval", 0);
	kprintf("\nprocess for stack eval:%d\n",pid);
	resume(pid);
	return 0;
}


