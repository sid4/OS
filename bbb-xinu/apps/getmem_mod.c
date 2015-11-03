/* getmem_mod.c - getmem_mod */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem_mod  -  Allocate heap storage, returning if allocation has occured or not
 *------------------------------------------------------------------------
 */

unsigned int getmem_mod(uint32 nbytes){
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*prev, *curr, *leftover;
	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return SYSERR;
	}
	nbytes = (uint32) roundmb(nbytes);	/* Use memblk multiples	*/
	prev = &memlist;
	curr = memlist.mnext;
	while (curr != NULL) {			/* Search free list	*/
		if (curr->mlength == nbytes) {	/* Block is exact match	*/
			prev->mnext = curr->mnext;
			memlist.mlength -= nbytes;
			restore(mask);
			return OK;

		} else if (curr->mlength > nbytes) { /* Split big block	*/
			leftover = (struct memblk *)((uint32) curr +
					nbytes);
			prev->mnext = leftover;
			leftover->mnext = curr->mnext;
			leftover->mlength = curr->mlength - nbytes;
			memlist.mlength -= nbytes;
			restore(mask);
			return OK;
		} else {			/* Move to next block	*/
			prev = curr;
			curr = curr->mnext;
		}
	}
	restore(mask);
	return SYSERR;
}
