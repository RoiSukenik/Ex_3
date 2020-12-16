#include "threads.h"

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

	thread_handle = createthread(
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

DWORD WINAPI main_of_sub_thread(LPVOID lpparam)
{

	thread_parameters* parameters_of_the_thread = (thread_parameters*)lpparam;
	HANDLE input_file_handle = NULL;
	HANDLE output_file_handle = NULL;
	int return_value;


}