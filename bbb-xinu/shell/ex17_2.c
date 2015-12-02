#include<xinu.h>
void send_rcv(char *remip_dot,uint32 remport,uint16 locport);

shellcmd ex17_2 (int nargs , char *args[]){
	send_rcv("192.168.1.100",50420,50420);
}
