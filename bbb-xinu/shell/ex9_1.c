#include<xinu.h>
shellcmd ex9_1 (int nargs , char *args[]){
	intmask mask;
	struct memblk *curr;
	mask = disable();
	curr = &memlist;
	while (curr != NULL) {
		kprintf("\naddress:%u length:%u\n",curr,curr->mlength);
		curr=curr->mnext;	
	}
	restore(mask);
	return 0;
}
