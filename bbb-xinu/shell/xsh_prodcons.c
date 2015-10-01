/* xsh_prodcons.c - xsh_prodcons */
#include <prodcons.h>
#include <string.h>
#include <stdlib.h>

/*--------------------------------------------------------------------------------------------
 * xsh_prodcons - create two processes and exchange data between them using a global variable
 *--------------------------------------------------------------------------------------------
 */

//Definition for global variable 'n'

 int n;
 sid32 consumed, produced;

/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

shellcmd xsh_prodcons (int nargs , char *args[]){
	
	//Argument verifications and validations

      	int   count;             //local varible to hold count


	/* Check argument count */

	if (nargs == 2) {
		/* Output info for '--help' argument */

		if (strncmp(args[1], "--help", 7) ==0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tExchange data between two processes\n");
		printf("\tExample for invocation of function: prodcons [integer]\n");
		printf("Options:\n");
		printf("\t--help\tdisplay this help and exit\n");
		
		}
		
		else if (atoi(args[1]) == 0){
		fprintf(stderr, "%s: Please enter only integer value\n",args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n",args[0]);
		return 1;
		}

		else{
			count = atoi(args[1]);
			
		}
	}

	else if(nargs<2){
		count = 2000;
	}
	else{
		fprintf(stderr, "%s:too many arguments\n", args[0]);
		return 1;
	}

	/* Initalize semaphores*/
	consumed = semcreate(1);
	produced = semcreate(0);
	
	//create the process producer and consumer and put them in ready queue
	resume(create(producer,1024, 20, "producer", 1, count ));
	resume(create(consumer,1024, 20, "consumer", 1, count ));	
	return 0;
	
}
