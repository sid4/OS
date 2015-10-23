#include <xinu.h>
#include <future.h>

syscall future_free(future* f){
	int status=0;
	status+=custom_queue_destroy(f->set_queue);
	status+=custom_queue_destroy(f->get_queue);
	status+=freemem((char *)f,sizeof(future));
	return status==3?OK:SYSERR;
}
