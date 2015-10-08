#include <xinu.h>
#include <future.h>
typedef unsigned int uint;
uint future_prod(future *fut) {

	//printf("in producer");
  int i, j;
  j = (int)fut;
	printf("\nflag:%d,state: %d, pid: %d\n, value:%d",(*fut).flag,(*fut).state,(*fut).pid,j);
  for (i=0; i<1000; i++) {
    j += i;
	
  }
	//printf("val in struct:%d",(*fut).value);	
	//printf("\nsetting future");
	//printf("\nj:%d",j);
  	
	//printf("val in struct:%d",(*fut).value);	
	//printf("\nj:%d",j);
	//printf("\nflag:%d,state: %d, pid: %d\n, value:%d",(*fut).flag,(*fut).state,(*fut).pid,j);
  return future_set(fut, &j);
}
