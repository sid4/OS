#include<xinu.h>
#include<future_nw.h>
#define OUT_MSG_LENGTH 40
#define IN_MSG_LENGTH 100
#define RCV_TIMEOUT 10000
void send_rcv_future(future_nw *future,char *remip_dot,uint32 remport,uint16 locport){
	uint32 remip;
	dot2ip(remip_dot,&remip);
	int32 slot=udp_register(remip,remport,locport);
	sleep(5);
	kprintf("\nINFO secondary_process: in secondary process\n");	
	if(slot==SYSERR){
		kprintf("\nINFO secondary_process:Failed to register a udp slot. Exiting");
		return;					
	}
	
	
		char out_msg[OUT_MSG_LENGTH]="message from xinu";
		kprintf("\nINFO secondary_process:sending msg:\"%s\" to linux vm",out_msg);
		int status_send=udp_send(slot,out_msg,OUT_MSG_LENGTH);
		if(status_send==SYSERR){
			kprintf("\nINFO secondary_process:Failed to send message");		
		}
		else{

		char *in_msg=(char*)getmem(IN_MSG_LENGTH);
		int i=0;
		for(;i<IN_MSG_LENGTH;i++){
			in_msg[i]='\0';		
		}

		int status_rcv=udp_recv(slot,in_msg,IN_MSG_LENGTH,RCV_TIMEOUT);

		if(status_rcv==TIMEOUT){
			kprintf("\nINFO secondary_process:Rcv timed out");
		}
		else if(status_rcv==SYSERR){
			kprintf("\nINFO secondary_process:Error occured while receiving msg");
		}			
		else{	
			kprintf("\nINFO secondary_process:response received from linux vm");
			kprintf("\nINFO secondary_process:setting future with received msg");
			future_set_nw(future,in_msg);
		}
		}
		
	
	int released=udp_release(slot);
	if(released==SYSERR){
		kprintf("\nINFO secondary_process:Failed to release the UDP slot");
	}
}
