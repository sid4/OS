/* getmem.c - getmem */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  getmem  -  Allocate heap storage, returning lowest word address
 *------------------------------------------------------------------------
 */
char  	*getmem(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	memblk	*previous, *current, *leftover;

	mask = disable();
	if (nbytes == 0) {
		restore(mask);
		return (char *)SYSERR;
	}

	previous = &memlist;
	current = memlist.mnext;
	while (current != NULL) {			/* Search free list	*/

		if (current->mlength == nbytes) {	/* Block is exact match	*/
			previous->mnext = current->mnext;
			memlist.mlength -= nbytes;
			restore(mask);
	return (char *)(current);

		} else if (current->mlength > nbytes) { /* Split big block	*/
			leftover = (struct memblk *)((uint32) current +
					nbytes);
			previous->mnext = leftover;
			leftover->mnext = current->mnext;
			leftover->mlength = current->mlength - nbytes;
			memlist.mlength -= nbytes;
			restore(mask);
	return (char *)(current);
		} else {			/* Move to next block	*/
			previous = current;
			current = current->mnext;
		}
	}
	restore(mask);
	return (char *)SYSERR;
}
