#include <xinu.h>
#include <future.h>
typedef unsigned int uint;
uint future_prod(future *fut) {
  int i, j;
  j = (int)fut;

  for (i=0; i<1000; i++) {
    j += i;
  }
  return future_set(fut, &j);
}
