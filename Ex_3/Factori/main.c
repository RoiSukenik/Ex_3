/*
	Authors - Roi Sukenik 312230741 ; Eldor Fadida 

	Project - ITOSP HomeWork 3

	Description - 



*/

#include "main.h"

int main(int argc, char* argv[])
{
	char* ptr;
	int amount_of_tasks = strtol(argv[3], &ptr, 10);
	Queue* que = extract_priorty_array(argv[2],amount_of_tasks);
	int amount_of_threads = strtol(argv[4], &ptr, 10);
	if (amount_of_threads > amount_of_tasks) { amount_of_threads = amount_of_tasks; }
	create_initilize_thandle_array(argv[1], que, amount_of_threads,amount_of_tasks); // resource free happnds here
	return STATUS_CODE_SUCCESS;
}