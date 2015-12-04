#include<xinu.h>
#include<future_nw.h>
void send_rcv_future(future_nw *future,char *remip_dot,uint32 remport,uint16 locport);

shellcmd ex17_2_2 (int nargs , char *args[]){
	if (nargs >=2 && nargs <= 4){
		if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
			printf("Usage: %s\n\n", args[0]);
			printf("Description:\n");
			printf("\tSecondary process sends a message to linux VM. Primary process waits for the reply from VM with the future. Once the future is resolved, primary process resumes and shows the response from linux vm\n");
			printf("\tTemplate for invocation of function: ex17_2_2 <remote ip> <local port> <remote port> \n");
			printf("\tExample for invocation of function: ex17_2_2 192.168.1.100 50420 50420\n");
			printf("Options:\n");
			printf("\t--help\tdisplay this help and exit\n");
			return 0;
		}
		else if(nargs < 4){
			fprintf(stderr, "%s:too few arguments\n", args[0]);
			return 0;
		}
		/*Code to be executed*/	
		future_nw *f_nw;
		f_nw = future_alloc_nw(FUTURE_EXCLUSIVE);
		kprintf("\nINFO main_process: creating secondary process\n");
		resume( create(send_rcv_future, 1024, 20, "send_rcv_future", 4, f_nw,args[1],atoi(args[3]),atoi(args[2])));
		char* msg;
		kprintf("\nINFO main_process: calling future get\n");
		int status=future_get_nw(f_nw,&msg);
		kprintf("\nINFO main_process: future resolved\n");
		if(status!=OK){
			printf("\nfuture failed to resolve");
		}
		kprintf("\nINFO main_process: Message received:%s",msg);		
		return 0;
	}
	else if(nargs<4){
		fprintf(stderr, "%s:too few arguments\n", args[0]);
		return 1;
	}
	else{
		fprintf(stderr, "%s:too many arguments\n", args[0]);
		return 1;
	}

	

}
