#include<xinu.h>
shellcmd ex9_1 (int nargs , char *args[]){
	intmask mask;
	struct memblk *prev, *curr;
	mask = disable();
	curr = &memlist;
	while (curr != NULL) {
		printf("address:%u length:%u",curr,curr->mlength);
		curr=curr->mnext;	
	}
	return 0;
}
