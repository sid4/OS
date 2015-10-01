/* xsh_reset.c - xsh_reset */

#include <xinu.h>
#include <stdio.h>

/*-----------------------------------------------
 * xsh_reset - Warm software reset of the Beaglebone
 *-----------------------------------------------
 */

shellcmd xsh_reset (int nargs , char *args[]){

	if (strncmp(args[1], "--help", 7) ==0){
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\tDo a software reset\n");
		printf("\tExample for invocation of function: reset\n");
		printf("Options:\n");
		printf("\t--help\tdisplay this help and exit\n");		
	}
	
	else if(strncmp(args[1], "--help", 7) != 0) {
		fprintf(stderr, "Try '%s --help' for more information\n",args[0]);
		return 1;
			
	}

	else{
		//Getting the address of the register
		volatile uint32*prm_rstctrl = (volatile uint32*) 0x44E00F00; 

		//Generate a warm software reset
		*prm_rstctrl = 0x01;
	}

	return 0;
}
