#ifndef THREADS_DOT_H
#define THREAD_DOT_H

#include <windows.h>


#define STATUS_CODE_FAILURE 1
#define STATUS_CODE_SUCCESS 0
#define MAX_INT 2147483647
#define WAIT_THREE_SECONDS 3000
#define WAIT_ONE_SECOND 1000
#define MAX_THREAD_NUMBER_ALLOWED 64
#define MIN_THREAD_NUMBER_ALLOWED 1

static HANDLE semaphore_handle;

/* this struct help us pass parameters to the threads*/
typedef struct _thread_parameters {
	char* input_file_path;
	char* output_file_path;
	int		task_start_index;
	int		task_end_index;
	int		security_key;

}thread_parameters;

/*
* Description - Used To Create a Simple Thread
* Parameters - Start Routine, Thread Parameters, Thread ID
* Returns - HANDLE to thread
*/
static HANDLE CreateThreadSimple
(
	LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id
);

/*
* Description - Points to entry function
* Parameters - Thread Params
* Returns - Pointer
*/
DWORD WINAPI Main_of_Sub_Thread(LPVOID lpParam);

#endif // !THREADS_DOT_H
