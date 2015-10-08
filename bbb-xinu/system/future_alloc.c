#include <xinu.h>
#include <future.h>

future* future_alloc(int future_flag){
	printf("\nallocating future");
	future *f=(future *)getmem(sizeof(future));
	(*f).flag=future_flag;
	(*f).state=FUTURE_EMPTY;
	(*f).pid=(pid32)-1;
/*
	(*f).value=(int *)getmem(sizeof(int));
	*(*f).value=0;
*/
	(*f).value=0;
	//printf("\nflag:%d,state: %d, pid: %d\n",(*f).flag,(*f).state),(*f).pid;
	return f;
}
