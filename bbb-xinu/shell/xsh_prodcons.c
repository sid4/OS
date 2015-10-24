/* xsh_prodcons.c - xsh_prodcons */
#include <prodcons.h>
#include <string.h>
#include <stdlib.h>
#include <future.h>
typedef unsigned int uint;


/*--------------------------------------------------------------------------------------------
 * xsh_prodcons - create two processes and exchange data between them using a global variable
 *--------------------------------------------------------------------------------------------
 */
/*
Functions for futures
*/
uint future_cons(future *fut);
uint future_prod(future *fut); 
//Definition for global variable 'n'

 int n;
 sid32 consumed, produced;

/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

shellcmd xsh_prodcons (int nargs , char *args[]){
	int run_futures=0;
	//Argument verifications and validations

      	int   count;             //local varible to hold count


	/* Check argument count */

	if (nargs == 2) {

		/* Output info for '--help' argument */
				/*
		if(strncmp(args[2], "-f", 3) !=0){
			fprintf(stderr, "%s:too many arguments\n", args[0]);
			return 1;
		}		
		*/
		if (strncmp(args[1], "--help", 7) ==0) {
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tExchange data between two processes\n");
		printf("\tExample for invocation of function: prodcons [integer]\n");
		printf("Options:\n");
		printf("\t--help\tdisplay this help and exit\n");
		printf("\t-f\trun the futures version\n");		
		}
		
		else if(strncmp(args[1], "-f", 3) ==0){
				run_futures=1;
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
	if(run_futures){
		future *f_exclusive, *f_shared, *f_queue;
		f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
		f_shared = future_alloc(FUTURE_SHARED);
		f_queue = future_alloc(FUTURE_QUEUE);
 		// Test FUTURE_EXCLUSIVE
	
		resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
		resume( create(future_prod, 1024, 20, "fprod1", 1, f_exclusive) );

		// Test FUTURE_SHARED

  		resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
		resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
		resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
		resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
		resume( create(future_prod, 1024, 20, "fprod2", 1, f_shared) );

		// Test FUTURE_QUEUE
			
		resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
		resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
		resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
		resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
		resume( create(future_prod, 1024, 20, "fprod3", 1, f_queue) );
		resume( create(future_prod, 1024, 20, "fprod4", 1, f_queue) );
		resume( create(future_prod, 1024, 20, "fprod5", 1, f_queue) );
		resume( create(future_prod, 1024, 20, "fprod6", 1, f_queue) );

	
	}
	else{		
		/* Initalize semaphores*/
		consumed = semcreate(1);
		produced = semcreate(0);
	
		//create the process producer and consumer and put them in ready queue
		resume(create(producer,1024, 20, "producer", 1, count ));
		resume(create(consumer,1024, 20, "consumer", 1, count ));	
	}
	return 0;
}
