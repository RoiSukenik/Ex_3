/*
	Authors - Roi Sukenik 312230741 ; Eldor Fadida 

	Project - ITOSP HomeWork 3

	Description - 



*/

#include "main.h"

static HANDLE CreateThreadSimple
(
	LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id
) {
	//initialize variable :
	HANDLE thread_handle;
	if ((NULL == p_start_routine) || (NULL == p_thread_id))				//check if the thread routine and id are valid.
	{
		printf("error when creating a thread , received null pointer\n");
		//return(error_creating_thread);
	}

	thread_handle = CreateThread(
		NULL,                /*  default security attributes */
		0,                   /*  use default stack size */
		p_start_routine,     /*  thread function */
		p_thread_parameters, /*  argument to thread function */
		0,                   /*  use default creation flags */
		p_thread_id);        /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("couldn't create thread\n");
		//return(error_creating_thread);
	}

	return thread_handle;
}
int main(int argc, char* argv[])
{

	clean_output_file("output.txt");
	char* ptr;
	int amount_of_tasks = strtol(argv[3], &ptr, 10);
	Queue* que = extract_priorty_array(argv[2],amount_of_tasks);
	int amount_of_threads = strtol(argv[4], &ptr, 10);
	HANDLE* th_array = create_initilize_thanlde_array(argv[1], que, amount_of_threads,amount_of_tasks);
	free(th_array);
	return STATUS_CODE_SUCCESS;
}