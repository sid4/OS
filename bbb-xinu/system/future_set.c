#include <xinu.h>
#include <future.h>

syscall future_set(future *f, int *value){
	intmask mask;//interrupt mask
	mask=disable();	
	if((*f).state==FUTURE_VALID){
		return SYSERR;	
		restore(mask);	
	}
	f->value=*value;
	(*f).state=FUTURE_VALID;

	while(!custom_queue_is_empty(f->get_queue)){
		//dequeue the suspended processes and resume it		
		resume(custom_queue_dequeue(f->get_queue));
	}
	restore(mask);
	return OK;
}
