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
	if (strncmp(args[1], "--help", 7) ==0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tPrint max stack usage of the process. \n");
		printf("\tTemplate for invocation: ex9_7 <max recursive function call>\n");
		printf("\tExample for invocation: ex9_7 10\n");
		printf("Options:\n");
		printf("\t--help\tdisplay this help and exit\n");
		}
	else{	
		iteration=atoi(args[1]);
		demo_pid=getpid();
		pid32 pid=create(stack_eval, 1024, 20, "stack_eval", 0);
		kprintf("\nprocess for stack eval:%d\n",pid);
		resume(pid);
	}
	}
	else{	
		iteration=100;
		demo_pid=getpid();
		pid32 pid=create(stack_eval, 1024, 20, "stack_eval", 0);
		kprintf("\nprocess for stack eval:%d\n",pid);
		resume(pid);
	}
	
	return 0;
}


