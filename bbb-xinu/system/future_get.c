#include <xinu.h>
#include <future.h>

syscall future_get(future *f, int *value){
	//checking pid
	if((*f).pid==-1){
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
