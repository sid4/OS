#include <xinu.h>
#include <future.h>

int exec_future_shared(future *f, int *value);
int exec_future_exclusive(future *f, int *value);

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
			kprintf("not implemented");
			return SYSERR;
		break;		
		}
		default:{
			kprintf("invalid future type");
			return SYSERR;		
		}
	}
}

int exec_future_shared(future *f, int *value){	
	if((*f).state=EMPTY){	
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
	if((*f).state=EMPTY){	
		(*f).state=FUTURE_WAITING;
	}
	while((*f).state==FUTURE_WAITING){
		sleepms(100);
	}
	*value=(*f).value;
	return 	OK;

}
