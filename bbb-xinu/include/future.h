#ifndef _FUTURE_H_
#define _FUTURE_H_

/*Define states*/

#define FUTURE_EMPTY 			0
#define FUTURE_WAITING 			1
#define FUTURE_VALID			2
/*modes of operation for future*/

#define FUTURE_EXCLUSIVE	1
#define FUTURE_SHARED		2
#define FUTURE_QUEUE		3
typedef struct{
	int *storage;
	int head;
	int tail;
	int size; 
} custom_queue;
typedef struct futent
{
	int value;
	int flag;
	int state;
	sid32 consumed;
	sid32 comsumer_exec_fin;
	pid32 pid;	
	custom_queue *set_queue;	
	custom_queue *get_queue;
}future;


/*Interface for system calls*/

future* future_alloc(int future_flags);
syscall future_free(future*);
syscall future_get(future*, int*);
syscall future_set(future*, int*);
/*Interface for custom queue*/

void custom_queue_init(int size, custom_queue *queue);

int custom_queue_enqueue(int procid,custom_queue *queue);

int custom_queue_dequeue(custom_queue *queue);

int custom_queue_destroy(custom_queue *queue);	
int custom_queue_is_empty(custom_queue *queue);

#endif /* _FUTURE_H_ */

	
