#include <xinu.h>
#include <future_nw.h>

future_nw* future_alloc_nw(int future_flag){
	//printf("\nallocating future");
	future_nw *f=(future_nw *)getmem(sizeof(future_nw));
	(*f).flag=future_flag;
	(*f).state=FUTURE_EMPTY;
	(*f).pid=(pid32)-1;
	f->set_queue=(custom_queue *)getmem(sizeof(custom_queue));
	f->get_queue=(custom_queue *)getmem(sizeof(custom_queue));
	custom_queue_init(10, f->set_queue);
	custom_queue_init(10, f->get_queue);
	if(future_flag==FUTURE_QUEUE){
		f->consumed=semcreate(1);
		f->comsumer_exec_fin=semcreate(1);
	}
/*
	(*f).value=(int *)getmem(sizeof(int));
	*(*f).value=0;
*/
	(*f).value=0;
	//printf("\nflag:%d,state: %d, pid: %d\n",(*f).flag,(*f).state),(*f).pid;
	return f;
}
