#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*getmem(
	  uint32	req_bytes		/* Size of memory requested	*/
	){
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*previous, *current, *remaining;
	mask = disable();
	if (req_bytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}
	previous = &memlist;
	current = memlist.mnext;
	while (current != NULL) {		/* Search free list	*/
		if (current->mlength > req_bytes) { 
			remaining = (struct memblk *)((uint32) current + req_bytes);
			previous->mnext = remaining;
			remaining->mnext = current->mnext;
			remaining->mlength = current->mlength - req_bytes;
			memlist.mlength -= req_bytes;
			restore(mask);
			return (char *)(current);
		} else {			
			previous = current;
			current = current->mnext;
		}
	}
	restore(mask);
	return (char *)SYSERR;
}
