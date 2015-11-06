#include<xinu.h>
shellcmd ex9_1 (int nargs , char *args[]){
if(nargs==2){	
	if (strncmp(args[1], "--help", 7) ==0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tPrints free list\n");
		printf("\tExample: ex9_1\n");
		printf("Options:\n");
		printf("\t--help\tdisplay this help and exit\n");
		}
}
else if(nargs==1){	
	intmask mask;
	struct memblk *curr;
	mask = disable();
	curr = &memlist;
	while (curr != NULL) {
		kprintf("\naddress:%u length:%u\n",curr,curr->mlength);
		curr=curr->mnext;	
	}
	restore(mask);
}
else{
		kprintf("Improper no. of args used, please use --help for guide");
	}
	return 0;
}
