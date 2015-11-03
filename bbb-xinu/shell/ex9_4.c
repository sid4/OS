#include<xinu.h>
#include <stdlib.h>
unsigned int exhaust_heap(){
	while(getmem_mod(100)!=SYSERR){
		kprintf("memory allocation from heap successful");	
	}
		kprintf("no more heap memory available");
	return OK;
}
unsigned int exhaust_stk(){
	while(getstk_mod(100)!=SYSERR){
		kprintf("stack memory allocation successful");	
	}
		kprintf("no more stack memory available");
	return OK;
}
shellcmd ex9_4 (int nargs , char *args[]){
	if(atoi(args[1])==0) {
		kprintf("demonstrating permanent heap allocation");
		resume(create(exhaust_heap, 1024, 20, "exhaust_heap", 0) );
	}
	else{
		kprintf("demonstrating permanent stack allocation");
		resume(create(exhaust_stk, 1024, 20, "exhaust_stk", 0) );
	}
	return 0;
}
