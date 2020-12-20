#ifndef THREADS_DOT_H
#define THREAD_DOT_H


#include "includes.h"
#include "prime_number_calculator.h"
#include "filesystem.h"
#include "Queue.h"

#define STATUS_CODE_FAILURE -1
#define STATUS_CODE_SUCCESS 0
#define MAX_INT 2147483647
#define WAIT_THREE_SECONDS 3000
#define WAIT_ONE_SECOND 1000
#define MAX_THREAD_NUMBER_ALLOWED 64
#define MIN_THREAD_NUMBER_ALLOWED 1
#define MAX_NUMBER_LENGTH 44
#define OUTPUT_FILE_NAME "output.txt"




/* this struct help us pass parameters to the threads*/
typedef struct _thread_parameters {
	char* input_file_path;
	char* output_file_path;
	int		task_start_index;
	int		task_end_index;

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

/*
* Description - what each thread should do while doing the task
* Parameters - task file handle, output file handle, task start index, tast end index
* Returns -STATUS CODE FAILURE | STATUS CODE SUCCESS
*/
int preform_task(HANDLE task_file_handle,
	HANDLE output_file_handle,
	int task_start_index,
	int task_end_index);
/*
* Description - Creates and initialize a thread parameter array
* Parameters - task file path, queue pointer, amount of threads
* Returns -pointer to array of thread params
*/
thread_parameters* create_initilize_tp_array(char* task_file_path, Queue* que, int amount_of_threads);

/*
* Description - Creates  a thread id array
* Parameters - pointer to thread param array, amount of threads
* Returns -pointer to array of thread ids
*/
DWORD* create_thread_id_array(thread_parameters* tp, int amount_of_threads);
/*
* Description - Creates  a thread handle array
* Parameters - pointer to task file path, queue pointer, amount of threads,amount of tasks
* Returns -pointer to array of thread handles
*/
HANDLE* create_initilize_thanlde_array(char* task_file_path, Queue* queue, int amount_of_threads, int amount_of_tasks);

/*
* Description - Free prime number list
* Parameters - Pointer to number list
* Returns - set list pointer to null
*/
void free_list(node* p_prime_list);

#endif // !THREADS_DOT_H
