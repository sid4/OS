#include<xinu.h>
#include<future_nw.h>
void send_rcv_future(future_nw *future,char *remip_dot,uint32 remport,uint16 locport);

shellcmd ex17_2_2 (int nargs , char *args[]){
	future_nw *f_nw;
	f_nw = future_alloc_nw(FUTURE_EXCLUSIVE);
	resume( create(send_rcv_future, 1024, 20, "fcons1", 4, f_nw,"192.168.1.100",50410,50410));
	char* msg;
	printf("\nwaiting\n");
	future_get_nw(f_nw,&msg);
}
