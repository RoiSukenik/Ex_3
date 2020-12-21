#include "threads.h"

int critical_read_code(char* tasked_string, int task_size,char* file_path,int task_start_index)
{
	HANDLE file_handle_read = NULL;
	file_handle_read = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle_read == INVALID_HANDLE_VALUE) {
		printf("Read request failed, Last Error = %s\n ", GetLastError());
		return STATUS_CODE_FAILURE;
	}

	int setfile_returned_value = SetFilePointer(
		file_handle_read,
		task_start_index,
		NULL,
		FILE_BEGIN);
	if (setfile_returned_value == INVALID_SET_FILE_POINTER) {
		printf("SetFilePointer failed in hte request, printing last ERROR = %d\n", GetLastError());
		CloseHandle(file_handle_read);
		return STATUS_CODE_FAILURE;
	}
	DWORD      nNumberOfBytesToRead = task_size;
	DWORD      lpNumberOfBytesRead;

	setfile_returned_value = ReadFile(file_handle_read,
		tasked_string,
		nNumberOfBytesToRead,
		&lpNumberOfBytesRead,
		NULL);
	if (setfile_returned_value == 0) {
		printf("ReadFile Failed on request, last error reveived : %d \n", GetLastError());
		free(tasked_string);
		CloseHandle(file_handle_read);
		return STATUS_CODE_FAILURE;
	}
	errno_t ret_val = CloseHandle(file_handle_read);
	if (ret_val == 0) printf("oh no");
}


int critical_write_code(char* token,int task_size, char* file_path, int task_start_index)
{
	DWORD write_bytes;						// for the sake of definition
	HANDLE file_handle_write = NULL;
	file_handle_write = CreateFileA(file_path, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (file_handle_write == INVALID_HANDLE_VALUE) {
		printf_s("Write request failed, Last Error = %s\n ", GetLastError());
		free(token);
		return STATUS_CODE_FAILURE;
	}
	int setfile_returned_value = SetFilePointer(
		file_handle_write,
		0,
		NULL,
		FILE_END);
	if (setfile_returned_value == INVALID_SET_FILE_POINTER) {
		printf("SetFilePointer failed in hte request, printing last ERROR = %d\n", GetLastError());
		free(token);
		CloseHandle(file_handle_write);
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
		CloseHandle(file_handle_write);
		return STATUS_CODE_FAILURE;
	}
	CloseHandle(file_handle_write);
}



int preform_task(char* file_path,Queue* que,Lock* lock)
{
	int task_start_index=0;
	int task_size = MAX_NUMBER_LENGTH * sizeof(int);
	bool QnotEmpty = TRUE;
	while (QnotEmpty)
	{
		// mutex --
		DWORD wait_code;
		wait_code = WaitForSingleObject(
			Mutex_que,		// handle to mutex
			WAIT_FIVE_SECOND);  // time-out interval
		if (wait_code != WAIT_OBJECT_0) {
			return STATUS_CODE_SUCCESS;
		}
		//mutexed area start:
		if (!Empty(que))
		{
			task_start_index = Top(que);
			Pop(que);
		}
		else {
			QnotEmpty = FALSE;
			return STATUS_CODE_SUCCESS;
		}
		//mutexed area end.
		int ret_val;
		ret_val = ReleaseMutex(Mutex_que);
		if (ret_val == 0) {
			printf("Release Mutex error: %d\n", GetLastError());
			return STATUS_CODE_FAILURE;
		} // mutex++

		char* tasked_string = NULL;
		tasked_string = (char*)malloc((task_size + 1) * sizeof(char));
		if (tasked_string == NULL) {
			printf("Memory Allocation Failed, this came from a sub thread !\n");
			return STATUS_CODE_FAILURE;
		}

		//locking for read !
		if (read_lock(lock) == STATUS_CODE_FAILURE) { free(tasked_string); return STATUS_CODE_FAILURE; }
		ret_val = 0;
		ret_val = critical_read_code(tasked_string, task_size, file_path, task_start_index);
		if (ret_val == STATUS_CODE_FAILURE) { return STATUS_CODE_FAILURE; }
		// releasing reader !
		if (read_release(lock) == STATUS_CODE_FAILURE) { free(tasked_string); return STATUS_CODE_FAILURE; }

		char* ptr;
		char* nexttoken = NULL;
		char* delim = "\r\n";
		char* token = strtok_s(tasked_string, delim, &nexttoken);
		int num;
		num = strtol(token, &ptr, 10);
		free(tasked_string);
		token = NULL;
		node* primes = divid_number_add_2_list(num);
		if (primes == NULL) { printf("primes failed\n"); return STATUS_CODE_FAILURE; }
		token = print_prime_list(primes);
		task_size = strlen(token);
		// locking for write
		if (write_lock(lock) == STATUS_CODE_FAILURE) { free(token); return STATUS_CODE_FAILURE; }
		ret_val = critical_write_code(token, task_size, file_path, task_start_index);
		if (ret_val == STATUS_CODE_FAILURE) { return STATUS_CODE_FAILURE; }
		// releasing for write
		if (write_release(lock) == STATUS_CODE_FAILURE) { free(token); return STATUS_CODE_FAILURE; }
	}
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
	int return_value;

	return_value = preform_task(
		parameters_of_the_thread->file_path,
		parameters_of_the_thread->que,
		parameters_of_the_thread->lock);

	if (return_value == STATUS_CODE_FAILURE) {
		printf("failed inside the thread logic");
		return STATUS_CODE_FAILURE;
	}
	return STATUS_CODE_SUCCESS;
}


thread_parameters* create_initilize_tp_array(char* task_file_path, Queue* que, int amount_of_threads)
{

	Lock* lock = InitializeLock(); 
	if (lock == NULL) {
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

	Mutex_que = NULL;
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
	



