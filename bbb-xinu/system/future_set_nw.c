#include <xinu.h>
#include <string.h>
#include <future_nw.h>

syscall future_set_nw(future_nw *f, char *value){
	int was_empty=0;	
	if(f->flag==FUTURE_QUEUE && custom_queue_is_empty(f->get_queue)){
		custom_queue_enqueue(getpid(),f->set_queue);
		was_empty=1;
		suspend(getpid());	
	}
	
	intmask mask;//interrupt mask
	mask=disable();		
	switch(f->flag){
		case FUTURE_EXCLUSIVE:
		case FUTURE_SHARED:{
			if((*f).state==FUTURE_VALID){
				return SYSERR;	
				restore(mask);	
			}
			(*f).value=value;
			(*f).state=FUTURE_VALID;
			while(!custom_queue_is_empty(f->get_queue)){
				//dequeue the suspended processes and resume it		
				resume(custom_queue_dequeue(f->get_queue));
			}
			break;		
		}
		case 	FUTURE_QUEUE:{
			wait(f->consumed);
			f->value=*value;
			(*f).state=FUTURE_VALID;
			if(!was_empty){
				//if the get queue was not empty
				resume(custom_queue_dequeue(f->get_queue));			
			}
			break;		
		}
		default:{
			kprintf("invalid future type");
			return SYSERR;		
		}
	}
	restore(mask);	
	return OK;
}
