#include <xinu.h>
#include <future.h>

syscall future_set(future *f, int *value){
	intmask mask;//interrupt mask
	mask=disable();	
	if((*f).state=FUTURE_VALID){
		return SYSERR;	
		restore(mask);	
	}
	f->value=*value;
	(*f).state=FUTURE_VALID;
	restore(mask);
	return OK;
}
