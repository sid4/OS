#include<xinu.h>
#include<future_nw.h>
#define OUT_MSG_LENGTH 40
#define IN_MSG_LENGTH 100
#define RCV_TIMEOUT 10000
void send_rcv_future(future_nw *future,char *remip_dot,uint32 remport,uint16 locport){
	uint32 remip;
	dot2ip(remip_dot,&remip);
	int32 slot=udp_register(remip,remport,locport);
	sleep(20);
	if(slot==SYSERR){
		kprintf("\nFailed to register a udp slot. Exiting");
		return;					
	}
	//int i=0;
	
		char out_msg[OUT_MSG_LENGTH]="message from xinu";
		int status_send=udp_send(slot,out_msg,OUT_MSG_LENGTH);
		if(status_send==SYSERR){
			kprintf("\nFailed to send message");		
		}
		else{
		char *in_msg=(char*)getmem(IN_MSG_LENGTH);
		int i=0;
		for(;i<IN_MSG_LENGTH;i++){
			in_msg[i]=0;		
		}

		int status_rcv=udp_recv(slot,&in_msg,IN_MSG_LENGTH,RCV_TIMEOUT);
		if(status_rcv==TIMEOUT){
			kprintf("\nRcv timed out");
		}
		else if(status_rcv==SYSERR){
				kprintf("\nError occured while receiving msg");
		}			
		else{
			future_set_nw(future,in_msg);
			//kprintf("\nrcvd msg:%s",in_msg);		
		}
		}
	//	i++;	
			
	
	int released=udp_release(slot);
	if(released==SYSERR){
		kprintf("\nFailed to release the UDP slot");
	}
}
