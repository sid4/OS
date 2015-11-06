#include<xinu.h>
#include <stdlib.h>
void print_freemem1();
unsigned int demo_heap(){
	kprintf("\nAllocating heap memory\n");
	getmem(200);
	return OK;
}
unsigned int demo_stk(){
	kprintf("\nAllocating stack memory\n");
	getstk(200);
	return OK;
}
int for_demo=-1;
/*
Pass commandline argument 0 for demonstrating permanent heap allocation
Pass commandline argument 1 for demonstrating permanent stack allocation
*/
shellcmd ex9_4 (int nargs , char *args[]){
	if(nargs==2){	
		if (strncmp(args[1], "--help", 7) ==0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tDemonstrate permanent heap/stack allocation\n");
		printf("\tExample for permanent heap allocation: ex9_4 0\n");
		printf("\tExample for permanent stack allocation: ex9_4 1\n");
		printf("Options:\n");
		printf("\t--help\tdisplay this help and exit\n");
		}
		else{
			if(atoi(args[1])==0) {
			pid32 pid=create(demo_heap, 1024, 20, "demo_heap", 0);
			for_demo=pid;
			kprintf("\ndemonstrating permanent heap allocation with process:%d\n",pid);
			print_freemem1();
			resume(pid);
			}
			else{
			kprintf("\ndemonstrating permanent stack allocation");
			print_freemem1();
			pid32 pid=create(demo_stk, 1024, 20, "demo_stk", 0);
			for_demo=pid;
			kprintf("\nWith process:%d\n",pid);
			resume(pid);
			//sleep(15);
			}
		}
	}
	else{
		kprintf("Improper no. of args used, please use --help for guide");
	}
	return 0;
}

void print_freemem1(){
	kprintf("\n*****free list start*****\n");
	struct memblk *curr;
	intmask mask = disable();
	curr = &memlist;
	while (curr != NULL) {
		printf("\naddress:%u length:%u\n",curr,curr->mlength);
		curr=curr->mnext;	
	}
	restore(mask);
	kprintf("\n*****free list end*****\n");
}
