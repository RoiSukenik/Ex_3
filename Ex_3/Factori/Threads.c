//#include "threads.h"
//
//static HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
//	LPVOID p_thread_parameters,
//	LPDWORD p_thread_id)
//{
//	//initialize variable :
//	HANDLE thread_handle;
//	if ((NULL == p_start_routine) || (NULL == p_thread_id))				//Check if the thread routine and id are valid.
//	{
//		printf("Error when creating a thread , Received null pointer\n");
//		//return(ERROR_CREATING_THREAD);
//	}
//
//	thread_handle = CreateThread(
//		NULL,                /*  default security attributes */
//		0,                   /*  use default stack size */
//		p_start_routine,     /*  thread function */
//		p_thread_parameters, /*  argument to thread function */
//		0,                   /*  use default creation flags */
//		p_thread_id);        /*  returns the thread identifier */
//
//	if (NULL == thread_handle)
//	{
//		printf("Couldn't create thread\n");
//		//return(ERROR_CREATING_THREAD);
//	}
//
//	return thread_handle;
//}
//
//DWORD WINAPI Main_of_Sub_Thread(LPVOID lpParam)
//{
//	DWORD wait_code;
//	if (semaphore_handle != NULL) {
//		wait_code = WaitForSingleObject(semaphore_handle, WAIT_ONE_SECOND);
//		if (wait_code != WAIT_OBJECT_0) {
//			return STATUS_CODE_FAILURE;
//		}
//	}
//	thread_parameters* parameters_of_the_thread = (thread_parameters*)lpParam;
//	HANDLE input_file_handle = NULL;
//	HANDLE output_file_handle = NULL;
//	int return_value;
//
//
//	printf("Thread reached\n"); //FIXME delete all debug prints 
//
//	input_file_handle = CreateFileA(parameters_of_the_thread->input_file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (input_file_handle == INVALID_HANDLE_VALUE) {
//		printf("Read request failed, Last Error = %s\n ", GetLastError);
//		return STATUS_CODE_FAILURE;
//	}
//	output_file_handle = CreateFileA(parameters_of_the_thread->output_file_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (output_file_handle == INVALID_HANDLE_VALUE) {
//		printf("Write request failed, Last Error = %s\n ", GetLastError);
//		// close all opened handles:
//		CloseHandle(input_file_handle);
//		return STATUS_CODE_FAILURE;
//	}
//
//	//return_value = ; FIX-ME;
//	CloseHandle(input_file_handle);
//	CloseHandle(output_file_handle);
//	if (return_value == STATUS_CODE_FAILURE) {
//		printf("failed inside the thread logic");
//		return STATUS_CODE_FAILURE;
//	}
//	return STATUS_CODE_SUCCESS;
//}