#include "threads.h"



int preform_task(HANDLE task_file_handle,
	HANDLE output_file_handle,
	int task_start_index,
	int task_end_index)
{
	int setfile_returned_value;
	DWORD write_bytes;
	char* tasked_string = NULL;
	int task_size = task_end_index - task_start_index;
	setfile_returned_value = SetFilePointer(
		task_file_handle,
		task_start_index,
		NULL,
		FILE_BEGIN);
	if (setfile_returned_value == INVALID_SET_FILE_POINTER) {
		printf("SetFilePointer failed in hte request, printing last ERROR = %d\n", GetLastError());
		return STATUS_CODE_FAILURE;
	}
	DWORD      nNumberOfBytesToRead = task_size;
	DWORD      lpNumberOfBytesRead;
	tasked_string = (char*)malloc((task_size + 1) * sizeof(char));
	if (tasked_string == NULL) {
		printf("Memory Allocation Failed, this came from a sub thread !\n");
		return STATUS_CODE_FAILURE;
	}
	setfile_returned_value = ReadFile(task_file_handle,
		tasked_string,
		nNumberOfBytesToRead,
		&lpNumberOfBytesRead,
		NULL);
	if (setfile_returned_value == 0) {
		printf("ReadFile Failed on request, last error reveived : %d \n", GetLastError());
		free(tasked_string);
		return STATUS_CODE_FAILURE;
	}
	char* ptr;
	char* nexttoken;
	char* token = strtok_s(setfile_returned_value, "\n", &nexttoken);
	int num;
	num = (int)strtol(token, &ptr, 10);
	token = NULL;
	node* primes = divid_number_add_2_list(num);
	tasked_string = print_prime_list(primes);
	free_list(primes);
	setfile_returned_value = SetFilePointer(
		output_file_handle,
		task_start_index,
		NULL,
		FILE_BEGIN);
	if (setfile_returned_value == INVALID_SET_FILE_POINTER) {
		printf("SetFilePointer failed in hte request, printing last ERROR = %d\n", GetLastError());
		free(tasked_string);
		return STATUS_CODE_FAILURE;
	}

	setfile_returned_value = WriteFile(output_file_handle,
		tasked_string,
		task_size,
		&write_bytes,
		NULL);
	if (setfile_returned_value == 0) {
		printf("Write File Falied");
		free(tasked_string);
		return STATUS_CODE_FAILURE;
	}
	free(tasked_string);
	return STATUS_CODE_SUCCESS;
}
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

DWORD WINAPI Main_of_Sub_Thread(LPVOID lpParam)
{

	thread_parameters* parameters_of_the_thread = (thread_parameters*)lpParam;
	HANDLE task_file_handle = NULL;
	HANDLE output_file_handle = NULL;
	int return_value;

	task_file_handle = CreateFileA(
		parameters_of_the_thread->input_file_path,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (task_file_handle == INVALID_HANDLE_VALUE) {
		char* error = NULL;
		sprintf_s(error, "%lu",sizeof(long), GetLastError);
		printf("Read request failed, Last Error = %s\n ", error);
		return STATUS_CODE_FAILURE;
	}


	output_file_handle = CreateFileA(
		parameters_of_the_thread->output_file_path,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (output_file_handle == INVALID_HANDLE_VALUE) {
		char* error = NULL;
		sprintf_s(error, "%lu", sizeof(long), GetLastError);
		printf_s("Write request failed, Last Error = %s\n ", error);
		// close all opened handles:
		CloseHandle(task_file_handle);
		return STATUS_CODE_FAILURE;
	}
	return_value = preform_task(
		task_file_handle,
		output_file_handle,
		parameters_of_the_thread->task_start_index,
		parameters_of_the_thread->task_end_index);

	CloseHandle(task_file_handle);
	CloseHandle(output_file_handle);
	if (return_value == STATUS_CODE_FAILURE) {
		printf("failed inside the thread logic");
		return STATUS_CODE_FAILURE;
	}
	return STATUS_CODE_SUCCESS;
}


thread_parameters* create_initilize_tp_array(char* task_file_path, Queue* que, int amount_of_threads)
{
	thread_parameters* array_of_thread_parameters = (thread_parameters*)malloc(amount_of_threads * sizeof(thread_parameters));
	if (array_of_thread_parameters == NULL) {
		printf("The pointer was not allocated correctly");
		exit(STATUS_CODE_FAILURE);
	}
	for (int i = 0; i < amount_of_threads; i++)
	{
		(array_of_thread_parameters + i)->input_file_path = task_file_path;
		(array_of_thread_parameters + i)->output_file_path = "output.txt";
		(array_of_thread_parameters + i)->task_start_index = Top(que);
		(array_of_thread_parameters + i)->task_end_index = Top(que) + MAX_NUMBER_LENGTH;//FIXME MAGIC NUMBER
		Pop(que);
		
	}
	return array_of_thread_parameters;
}

DWORD* create_thread_id_array(thread_parameters* tp, int amount_of_threads)
{
	DWORD* array_of_thread_ids = (DWORD*)malloc(amount_of_threads * sizeof(DWORD));
	if (array_of_thread_ids == NULL) {
		printf("The pointer was not allocated correctly");
		free(tp);
		exit(STATUS_CODE_FAILURE);
	}
	return array_of_thread_ids;
}
HANDLE* create_initilize_thanlde_array(char* task_file_path,Queue* queue,int amount_of_threads,int amount_of_tasks)
{	
	
	HANDLE* array_of_handles_to_threads = (HANDLE*)malloc(amount_of_threads * sizeof(HANDLE));
	if (array_of_handles_to_threads == NULL) {
		printf("The pointer was not allocated correctly");
		exit(STATUS_CODE_FAILURE);
	}
	while(amount_of_tasks > 0)
	{
		thread_parameters* tp = create_initilize_tp_array(task_file_path, queue, amount_of_tasks);
		DWORD* thread_id_array = create_thread_id_array(tp, amount_of_tasks);
		int i = 0;
		while(amount_of_threads > 0)
		{
			*(array_of_handles_to_threads + i) = NULL;
			*(array_of_handles_to_threads + i) = CreateThreadSimple(Main_of_Sub_Thread, tp+i, thread_id_array+i);
			if (NULL == *(array_of_handles_to_threads + i))
			{
				printf("Couldn't create thread, freeing previously created threads and memory allocations, bye.\n");
				// CloseHandles
				for (int j = 0; j < i; j++) { CloseHandle(*(array_of_handles_to_threads + j)); }
				// Free pointers
				free(array_of_handles_to_threads); free(thread_id_array);free(tp);
				exit(STATUS_CODE_FAILURE);
			}
			DWORD wait_code;
			wait_code = WaitForMultipleObjects
			(amount_of_threads,
				array_of_handles_to_threads,
				TRUE,
				WAIT_THREE_SECONDS); //FIXME check with finite time
			if (wait_code != WAIT_OBJECT_0)
			{				// handling the failure 
				if (wait_code == WAIT_TIMEOUT) { printf("threads wait failed (Handle timeout), closing everything !\n"); }
				else { printf("threads wait failed (Handle failure), closing everything !\n"); }
				// CloseHandles
				for (int i = 0; i < amount_of_threads; i++) { CloseHandle(*(array_of_handles_to_threads + i)); }
				// Free pointers
				free(array_of_handles_to_threads); free(thread_id_array); free(tp);
				exit(STATUS_CODE_FAILURE);
			}
			amount_of_tasks = amount_of_tasks - amount_of_threads;
			
		}
		

		// CloseHandles
		for (int i = 0; i < amount_of_threads; i++) { CloseHandle(*(array_of_handles_to_threads + i)); }
		// Free pointers
		free(array_of_handles_to_threads);
		free(thread_id_array);
		free(tp);
	}
}
	


