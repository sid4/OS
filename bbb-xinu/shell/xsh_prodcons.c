/* xsh_prodcons.c - xsh_prodcons */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <prodcons.h>
#include <stdlib.h>

/*--------------------------------------------------------------------------------------------
 * xsh_prodcons - create two processes and exchange data between them using a global variable
 *--------------------------------------------------------------------------------------------
 */

//Definition for global variable 'n'

int n;

/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

shellcmd xsh_prodcons (int nargs , char *args[]){
	
	//Argument verifications and validations

      	int count = 2000;             //local varible to hold count
	

	/* Check argument count */

	if (nargs == 2) {
		/* Output info for '--help' argument */

		if (strncmp(args[1], "--help", 7) == 0 && strlen(args[1]) == 7) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tExchange data between two processes\n");
		printf("\tExample for invocation of function: prodcons [integer]\n");
		printf("Options:\n");
		printf("\t--help\tdisplay this help and exit\n");
		return 0;
		}
		
		else if (atoi(args[1]) == 0){
		fprintf(stderr, "%s: Please enter only integer value\n",arg[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",args[0]);
		return 1;
		}

		else{
			count = atoi(args[1]);
		}
	}

	else if(nargs<2){
		fprintf(stderr, "%s:too few arguments\n", args[0]);
		return 1;
	}
	else{
		fprintf(stderr, "%s:too many arguments\n", args[0]);
		return 1;
	}
	
	//create the process producer and consumer and put them in ready queue
	resume(create(producer,1024, 20, "producer", 1, count ));
	resume(create(consumer,1024, 20, "consumer", 1, count ));
	
}
