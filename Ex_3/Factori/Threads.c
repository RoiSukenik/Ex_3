#include "threads.h"



int preform_task(HANDLE file_handle_read, HANDLE file_handle_write,
	Queue* que,
	Lock* lock
)
{
	int task_start_index=0;
	int task_size = MAX_NUMBER_LENGTH * sizeof(int);

	DWORD wait_code;					
	wait_code = WaitForSingleObject(
		Mutex_que,		// handle to mutex
		WAIT_TWO_MINUTES);  // time-out interval
	if (wait_code != WAIT_OBJECT_0) {
		return STATUS_CODE_FAILURE;
	}
	//mutexed area start:
	if (!Empty(que))
	{	
		task_start_index = Top(que);
		Pop(que);
	}
	//mutexed area end.
	if (!ReleaseMutex(Mutex_que))
	{
		printf("Release Mutex error: %d\n", GetLastError());
		return STATUS_CODE_FAILURE;
	}

	int setfile_returned_value;
	DWORD write_bytes;						// for the sake of definition
	char* tasked_string = NULL;

	setfile_returned_value = SetFilePointer(
		file_handle_read,
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
	//locking for read !
	if (read_lock(lock) == STATUS_CODE_FAILURE) { free(tasked_string); return STATUS_CODE_FAILURE; }
	setfile_returned_value = ReadFile(file_handle_read,
		tasked_string,
		nNumberOfBytesToRead,
		&lpNumberOfBytesRead,
		NULL);
	if (setfile_returned_value == 0) {
		printf("ReadFile Failed on request, last error reveived : %d \n", GetLastError());
		free(tasked_string);
		return STATUS_CODE_FAILURE;
	}
	// releasing reader !
	if (read_release(lock) == STATUS_CODE_FAILURE) { free(tasked_string); return STATUS_CODE_FAILURE; }
	
	char* ptr;
	char* nexttoken;
	char* delim = "\r\n";
	char* token = strtok_s(tasked_string, delim, &nexttoken);

	int num;
	num = (int)strtol(token, &ptr, 10);
	token = NULL;
	node* primes = divid_number_add_2_list(num);
	free(tasked_string);
	token = print_prime_list(primes);
	free_list(primes);
	
	
	if (write_lock(lock) == STATUS_CODE_FAILURE) { free(token); return STATUS_CODE_FAILURE; }
	setfile_returned_value = SetFilePointer(
		file_handle_write,
		task_start_index,
		NULL,
		FILE_END);
	if (setfile_returned_value == INVALID_SET_FILE_POINTER) {
		printf("SetFilePointer failed in hte request, printing last ERROR = %d\n", GetLastError());
		free(token);
		return STATUS_CODE_FAILURE;
	}
	
	setfile_returned_value = WriteFile(file_handle_write,
		token,
		task_size,
		&write_bytes,
		NULL);
	if (setfile_returned_value == 0) {
		printf("Write File Falied");
		free(token);
		return STATUS_CODE_FAILURE;
	}
	if(write_release(lock) == STATUS_CODE_FAILURE) { free(token); return STATUS_CODE_FAILURE; }


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
	HANDLE file_handle_read = NULL;
	HANDLE file_handle_write = NULL;
	int return_value;

	printf("Thread reached\n");//Delete me

	file_handle_read = CreateFileA(parameters_of_the_thread->file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle_read == INVALID_HANDLE_VALUE) {
		printf("Read request failed, Last Error = %s\n ", GetLastError);
		return STATUS_CODE_FAILURE;
	}


	file_handle_write = CreateFileA(parameters_of_the_thread->file_path,GENERIC_WRITE,FILE_SHARE_WRITE,	NULL, OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (file_handle_write == INVALID_HANDLE_VALUE) {
		printf_s("Write request failed, Last Error = %s\n ", GetLastError);
		// close all opened handles:
		CloseHandle(file_handle_read);
		return STATUS_CODE_FAILURE;
	}

	return_value = preform_task(
		file_handle_read,
		file_handle_write,
		parameters_of_the_thread->que,
		parameters_of_the_thread->lock);

	CloseHandle(file_handle_read);
	CloseHandle(file_handle_write);
	if (return_value == STATUS_CODE_FAILURE) {
		printf("failed inside the thread logic");
		return STATUS_CODE_FAILURE;
	}
	return STATUS_CODE_SUCCESS;
}


thread_parameters* create_initilize_tp_array(char* task_file_path, Queue* que, int amount_of_threads)
{

	Lock* lock = InitializeLock(); 
	if (lock = NULL) {
		printf("The pointer was not allocated correctly");
		DestroyQueue(que);
		exit(STATUS_CODE_FAILURE);
	}

	thread_parameters* array_of_thread_parameters = (thread_parameters*)malloc(amount_of_threads * sizeof(thread_parameters));
	if (array_of_thread_parameters == NULL) {
		printf("The pointer was not allocated correctly");
		DestroyQueue(que);
		exit(STATUS_CODE_FAILURE);
	}
	for (int i = 0; i < amount_of_threads; i++)
	{
		(array_of_thread_parameters + i)->file_path = task_file_path;
		(array_of_thread_parameters + i)->que = que;
		(array_of_thread_parameters + i)->lock = lock;	
	}
	return array_of_thread_parameters;
}

DWORD* create_thread_id_array(thread_parameters* tp, int amount_of_threads)
{
	DWORD* array_of_thread_ids = (DWORD*)malloc(amount_of_threads * sizeof(DWORD));
	if (array_of_thread_ids == NULL) {
		printf("The pointer was not allocated correctly");		
		DestroyQueue(tp->que);
		DestroyLock(tp->lock);
		free(tp);
		exit(STATUS_CODE_FAILURE);
	}
	return array_of_thread_ids;
}
void create_initilize_thandle_array(char* task_file_path,Queue* queue,int amount_of_threads,int amount_of_tasks)
{	
	HANDLE* array_of_handles_to_threads = (HANDLE*)malloc(amount_of_threads * sizeof(HANDLE));
	if (array_of_handles_to_threads == NULL) {
		printf("The pointer was not allocated correctly");
		DestroyQueue(queue);
		exit(STATUS_CODE_FAILURE);
	}

	thread_parameters* tp = create_initilize_tp_array(task_file_path, queue, amount_of_tasks);
	DWORD* thread_id_array = create_thread_id_array(tp, amount_of_tasks);


	Mutex_que = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

	if (Mutex_que == NULL)
	{
		printf("CreateMutex error: %d\n", GetLastError());
		DestroyQueue(queue);
		return NULL;
	}


	for(int i= 0;i<amount_of_threads;i++)
	{
		*(array_of_handles_to_threads + i) = NULL;
		*(array_of_handles_to_threads + i) = CreateThreadSimple(Main_of_Sub_Thread, tp + i, thread_id_array + i);
		if (NULL == *(array_of_handles_to_threads + i))
		{
			printf("Couldn't create thread, freeing previously created threads and memory allocations, bye.\n");
			// CloseHandles
			for (int j = 0; j < i; j++) { CloseHandle(*(array_of_handles_to_threads + j)); }
			// Free pointers
			free(array_of_handles_to_threads);
			free(thread_id_array);
			DestroyQueue(tp->que);
			DestroyLock(tp->lock);
			free(tp);
			exit(STATUS_CODE_FAILURE);
		}
	}


	DWORD wait_code;
	wait_code = WaitForMultipleObjects(
		amount_of_threads,
		array_of_handles_to_threads,
		TRUE,
		WAIT_TWO_MINUTES);
	if (wait_code != WAIT_OBJECT_0)
	{				// handling the failure 
		if (wait_code == WAIT_TIMEOUT) { printf("threads wait failed (Handle timeout), closing everything !\n"); }
		else { printf("threads wait failed (Handle failure), closing everything !\n"); }
		// CloseHandles
		for (int i = 0; i < amount_of_threads; i++) { CloseHandle(*(array_of_handles_to_threads + i)); }
		// Free pointers
		free(array_of_handles_to_threads); 
		free(thread_id_array); 
		DestroyQueue(tp->que);
		DestroyLock(tp->lock);
		free(tp);
		exit(STATUS_CODE_FAILURE);
	}
			

		

	// CloseHandles
	for (int i = 0; i < amount_of_threads; i++) { CloseHandle(*(array_of_handles_to_threads + i)); }
	// Free pointers
	free(array_of_handles_to_threads);
	free(thread_id_array);
	DestroyQueue(tp->que);
	DestroyLock(tp->lock);
	free(tp);
}
	



