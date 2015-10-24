#include <xinu.h>
#include <future.h>

int exec_future_shared(future *f, int *value);
int exec_future_exclusive(future *f, int *value);
int exec_future_queue(future *f, int *value);

syscall future_get(future *f, int *value){
	int status;
	switch(f->flag){
		case 	FUTURE_EXCLUSIVE:{
		status=exec_future_exclusive(f,value);
		if(status==SYSERR){
			return SYSERR;		
		}
		break;		
		}
		case 	FUTURE_SHARED:{
			status=exec_future_shared(f,value);
			if(status==SYSERR){
				return SYSERR;		
			}
		break;		
		}
		case 	FUTURE_QUEUE:{
			status=exec_future_queue(f,value);
			if(status==SYSERR){
				return SYSERR;		
			}
		break;		
		}
		default:{
			kprintf("invalid future type");
			return SYSERR;		
		}
	}
	return OK;
}


int exec_future_queue(future *f, int *value){	
	if((*f).state==FUTURE_EMPTY){	
		(*f).state=FUTURE_WAITING;
	}
	if((*f).state==FUTURE_WAITING){
		if(custom_queue_is_empty(f->set_queue)){
			//set_queue is empty
			int enqued=custom_queue_enqueue(getpid(),f->get_queue);
			if(enqued==SYSERR){
				kprintf("failed to enque the process in get_queue:%d",getpid());
				return SYSERR;	
			}
			int status=suspend(getpid());
			if(status==SYSERR){
				kprintf("failed to suspend the process:%d",getpid());
				return SYSERR;
			}	
		}
		else{
			//set_queue has processes enqueued
			int pid=custom_queue_dequeue(f->set_queue);
			if(pid==-1){
				kprintf("failed to dequeue");
				return SYSERR;	
			}
			resume(pid);
			wait(f->comsumer_exec_fin);
			while((*f).state==FUTURE_WAITING){
				sleepms(100);
			}
							
		}
	}

	*value=f->value;
	//reset future
	f->state=FUTURE_WAITING;	
	//signal consumed
	signal(f->comsumer_exec_fin);
	signal(f->consumed);
	return 	OK;
}


int exec_future_shared(future *f, int *value){	
	if((*f).state==FUTURE_EMPTY){	
		(*f).state=FUTURE_WAITING;
	}
	if((*f).state==FUTURE_WAITING){
		int enqued=custom_queue_enqueue(getpid(),f->get_queue);
		if(enqued==SYSERR){
			printf("failed to enque the process in get_queue:%d",getpid());
			return SYSERR;	
		}
		int status=suspend(getpid());
		if(status==SYSERR){
			printf("failed to suspend the process:%d",getpid());
			return SYSERR;
		}
	}
	*value=(*f).value;
	return 	OK;
}

int exec_future_exclusive(future *f, int *value){
	//checking pid
	if((*f).pid==-1){
	//setting pid
	(*f).pid=(pid32)getpid();
	}
	if((*f).flag==FUTURE_EXCLUSIVE){
		if((*f).pid!=(pid32)getpid()){
			return SYSERR;		
		}	
	}
	if((*f).state==FUTURE_EMPTY){	
		(*f).state=FUTURE_WAITING;
	}
	while((*f).state==FUTURE_WAITING){
		sleepms(100);
	}
	*value=(*f).value;
	return 	OK;

}
