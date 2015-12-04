#include<xinu.h>
void send_rcv(char *remip_dot,uint32 remport,uint16 locport);

shellcmd ex17_2 (int nargs , char *args[]){
	if (nargs >=2 && nargs <= 4){
		if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
			printf("Usage: %s\n\n", args[0]);
			printf("Description:\n");
			printf("\tSends and receives messages to/from linux vm\n");
			printf("\tTemplate for invocation of function: ex17_2 <remote ip> <local port> <remote port> \n");
			printf("\tExample for invocation of function: ex17_2 192.168.1.100 50420 50420\n");
			printf("Options:\n");
			printf("\t--help\tdisplay this help and exit\n");
			return 0;
		}
		else if(nargs < 4){
			fprintf(stderr, "%s:too few arguments\n", args[0]);
			return 0;
		}
		send_rcv(args[1],atoi(args[3]),atoi(args[2]));
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
