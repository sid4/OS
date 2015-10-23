#include <xinu.h>
#include <future.h>

future* future_alloc(int future_flag){
	printf("\nallocating future");
	future *f=(future *)getmem(sizeof(future));
	(*f).flag=future_flag;
	(*f).state=FUTURE_EMPTY;
	(*f).pid=(pid32)-1;
	f->set_queue=(custom_queue *)getmem(sizeof(custom_queue));
	f->get_queue=(custom_queue *)getmem(sizeof(custom_queue));
	custom_queue_init(10, f->set_queue);
	custom_queue_init(10, f->get_queue);
/*
	(*f).value=(int *)getmem(sizeof(int));
	*(*f).value=0;
*/
	(*f).value=0;
	//printf("\nflag:%d,state: %d, pid: %d\n",(*f).flag,(*f).state),(*f).pid;
	return f;
}
