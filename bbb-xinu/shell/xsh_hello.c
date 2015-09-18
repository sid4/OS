/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *Syntax: 
 *   xsh$ hello <string>
 *Output 
 *  Hello <string>, Welcome to the world of Xinu!!
 *------------------------------------------------------------------------
 */

shellcmd xsh_hello(int nargs, char *args[]) {
	if (nargs == 2){
		if (strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tSays Welcome World to the given argument\n");
		printf("\tExample for invocation of function: hello <string>\n");
		printf("Options:\n");
		printf("\t--help\tdisplay this help and exit\n");
		return 0;
	}
		printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
		return 0;
	}
	else if(nargs<2){
		fprintf(stderr, "%s:too few arguments\n", args[0]);
		return 1;
	}
	else{
		fprintf(stderr, "%s:too many arguments\n", args[0]);
		return 1;
	}

}
