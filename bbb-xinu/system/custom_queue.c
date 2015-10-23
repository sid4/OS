#include<xinu.h>
#include<future.h>
#include<stdlib.h>
/**
Initialize Queue
**/
void custom_queue_init(int size, custom_queue *queue){
	queue->storage=(int *)getmem(size*sizeof(int));
	queue->head=0;
	queue->tail=0;
	queue->size=size;
}

/**
Enqueue Operation
**/
int custom_queue_enqueue(int procid,custom_queue *queue){
	//kprintf("enqueue diff: %d",queue->head-queue->tail);
	if(queue->head-queue->tail==1||queue->head-queue->tail==queue->size){	
		//kprintf("Queue full");
		return -1;	
	}
	else{
		queue->storage[queue->tail]=procid;
		queue->tail=(queue->tail+1)%queue->size;
	}
	return 1;		
}
/**
Check for empty queue
**/
int custom_queue_is_empty(custom_queue *queue){
	return 	queue->head==queue->tail;
}
/**
Dequeue operation
**/
int custom_queue_dequeue(custom_queue *queue){
	if(custom_queue_is_empty(queue)){
		kprintf("Queue empty");
		return -1;
	} 
	else{
		int ret=queue->storage[queue->head];	
		queue->head=(queue->head+1)%queue->size;
		return ret;	
	}
}

/**
Destroy operation
**/

int custom_queue_destroy(custom_queue *queue){
	int status=0;
	status+=freemem((char *)queue->storage,queue->size*sizeof(int));
	status+=freemem((char *)queue,sizeof(queue));
	return status==2?OK:SYSERR;	
}
