#include <xinu.h>
#include <future.h>
typedef unsigned int uint;
uint future_cons(future *fut) {
	//printf("in consumer");
  int i, status;
  status = future_get(fut, &i);
  if (status < 1) {
    printf("future_get failed\n");
    return -1;
  }
  printf("it produced %d,%d\n", i,fut->value);
  return OK;
}
