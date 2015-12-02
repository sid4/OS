#include <xinu.h>
#include <future_nw.h>

syscall future_free_nw(future_nw* f){
	int status=0;
	status+=custom_queue_destroy(f->set_queue);
	status+=custom_queue_destroy(f->get_queue);
	status+=freemem((char *)f->value,strlen(f->value));
	status+=freemem((char *)f,sizeof(future_nw));
	return status==4?OK:SYSERR;
}
