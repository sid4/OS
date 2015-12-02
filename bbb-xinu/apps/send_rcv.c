#include<xinu.h>
#define OUT_MSG_LENGTH 40
#define IN_MSG_LENGTH 100
#define RCV_TIMEOUT 10000
void send_rcv(char *remip_dot,uint32 remport,uint16 locport){
	uint32 remip;
	dot2ip(remip_dot,&remip);
	int32 slot=udp_register(remip,remport,locport);
	char mac[ETH_ADDR_LEN];	
	int arp_status=arp_resolve(remip,&mac);
	printf("mac address resolved :%s",arp_status==OK?"true":"false");
	printf("resolved mac address:%s",mac);
	if(slot==SYSERR){
		kprintf("\nFailed to register a udp slot. Exiting");
		return;					
	}
	int i=0;
	while(i<10){
		char out_msg[OUT_MSG_LENGTH]="message from xinu";
		int status_send=udp_send(slot,out_msg,OUT_MSG_LENGTH);
		if(status_send==SYSERR){
			kprintf("\nFailed to send message");		
		}
		else{
		char in_msg[IN_MSG_LENGTH]="";
		int status_rcv=udp_recv(slot,&in_msg,IN_MSG_LENGTH,RCV_TIMEOUT);
		if(status_rcv==TIMEOUT){
			kprintf("\nRcv timed out");
		}
		else if(status_rcv==SYSERR){
				kprintf("\nError occured while receiving msg");
		}			
		else{
			kprintf("\nrcvd msg:%s",in_msg);		
		}
		}
		i++;	
			
	}
	int released=udp_release(slot);
	if(released==SYSERR){
		kprintf("\nFailed to release the UDP slot");
	}
}
