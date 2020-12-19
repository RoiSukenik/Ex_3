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
	char* output = OUTPUT_FILE_NAME;
	clean_output_file("output.txt");
	Queue* que = extract_priorty_array(argv[2]);
	int amount_of_threads = atoi(argv[4]);

	thread_parameters* array_of_thread_parameters = (thread_parameters*)malloc(amount_of_threads * sizeof(thread_parameters));
	if (array_of_thread_parameters == NULL) {
		printf("The pointer was not allocated correctly");
		exit(STATUS_CODE_FAILURE);
	}

	HANDLE* array_of_handles_to_threads = (HANDLE*)malloc(amount_of_threads * sizeof(HANDLE));
	if (array_of_handles_to_threads == NULL) {
		printf("The pointer was not allocated correctly");
		free(array_of_thread_parameters);
		exit(STATUS_CODE_FAILURE);
	}

	DWORD* array_of_thread_ids = (DWORD*)malloc(amount_of_threads * sizeof(DWORD));
	if (array_of_thread_ids == NULL) {
		printf("The pointer was not allocated correctly");
		free(array_of_thread_parameters);
		free(array_of_handles_to_threads);
		exit(STATUS_CODE_FAILURE);
	}
	for (int i = 0; i < amount_of_threads; i++)
	{	
		(array_of_thread_parameters + i)->input_file_path = argv[1];
		(array_of_thread_parameters + i)->output_file_path = "output.txt";

		(array_of_thread_parameters + i)->task_start_index = Top(que);
		Pop(que);
		(array_of_thread_parameters + i)->task_end_index = Top(que) - 2;//FIXME MAGIC NUMBER

	}
	for (int i = 0;i < amount_of_threads; i++)
	{
		*(array_of_handles_to_threads + i) = NULL;
		*(array_of_handles_to_threads + i) = CreateThreadSimple(Main_of_Sub_Thread, array_of_thread_parameters + i, (array_of_thread_ids + i));
		if (NULL == *(array_of_handles_to_threads + i))
		{
			printf("Couldn't create thread, freeing previously created threads and memory allocations, bye.\n");
			// CloseHandles
			for (int j = 0; j < i; j++) { CloseHandle(*(array_of_handles_to_threads + j)); }
			// Free pointers
			free(array_of_handles_to_threads); free(array_of_thread_ids); /*free(output_path);
*/			free(array_of_thread_parameters);
			exit(STATUS_CODE_FAILURE);
		}
		DWORD wait_code;
		wait_code = WaitForMultipleObjects(amount_of_threads, array_of_handles_to_threads, TRUE, WAIT_THREE_SECONDS); //FIXME check with finite time
		if (wait_code != WAIT_OBJECT_0)
		{				// handling the failure 
			if (wait_code == WAIT_TIMEOUT) { printf("threads wait failed (Handle timeout), closing everything !\n"); }
			else { printf("threads wait failed (Handle failure), closing everything !\n"); }
			// CloseHandles
			for (int i = 0; i < amount_of_threads; i++) { CloseHandle(*(array_of_handles_to_threads + i)); }
			// Free pointers
			free(array_of_handles_to_threads); free(array_of_thread_ids); /*free(output_path);*/
			free(array_of_thread_parameters); 
			exit(STATUS_CODE_FAILURE);
		}

		// CloseHandles
		for (int i = 0; i < amount_of_threads; i++) { CloseHandle(*(array_of_handles_to_threads + i)); }
		// Free pointers
		free(array_of_handles_to_threads);
		free(array_of_thread_ids);
		/*free(output_path);*/
		free(array_of_thread_parameters);
	}
	return STATUS_CODE_SUCCESS;
}