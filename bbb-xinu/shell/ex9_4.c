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
shellcmd ex9_4 (int nargs , char *args[]){
	if(atoi(args[1])==0) {
		pid32 pid=create(demo_heap, 1024, 20, "demo_heap", 0);
		kprintf("\ndemonstrating permanent heap allocation with process:%d\n",pid);
		print_freemem1();
		resume(pid);
		sleep(15);
	}
	else{
		kprintf("\ndemonstrating permanent stack allocation");
		print_freemem1();
		pid32 pid=create(demo_stk, 1024, 20, "demo_stk", 0);
		kprintf("\nWith process:%d\n",pid);
		resume(pid);
		sleep(15);
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
