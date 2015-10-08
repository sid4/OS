#include <xinu.h>
#include <future.h>

syscall future_free(future* f){
	return freemem((char *)f,sizeof(future));
}
