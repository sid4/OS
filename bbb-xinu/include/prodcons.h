/* prodcons1.h */
#include <xinu.h>

/* Global variable for Producer Consumer */
extern int n; /* Decleration of global variable n */

/* Declare the required semaphores*/
extern sid32 consumed, produced;

/* function prototype */
void consumer (int count);
void producer (int count);
