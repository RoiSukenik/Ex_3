#include "filesystem.h"

Queue* extract_priorty_array(char* input_path_priorty, int amount_of_tasks)
{
	Queue* queue = InitializeQueue();
	if (queue == NULL) {printf("failed to initiallize queue");}
	errno_t retval;
	FILE* p_stream;

	char line [MAX_NUMBER_LENGTH];
	char* p_line = NULL;
	char* left_token = NULL;
	char* ptr ;
	retval = fopen_s(&p_stream, input_path_priorty, "r");
	char* delim = "\n";
	if (0 != retval && p_stream == 0) { printf("Failed to open file.\n"); fclose(p_stream); exit(STATUS_CODE_FAILURE); }
	for (int i = 0; i < amount_of_tasks; i++)
	{
		
		
		fgets(line,MAX_NUMBER_LENGTH,p_stream);
		p_line = strtok_s(line, delim, &left_token);
		
		int num;
		num = strtol(p_line, &ptr, 10);
		if (Push(queue, num) == STATUS_CODE_FAILURE) { 
			printf("failed to push into queue");
			DestroyQueue(queue);
		}
		
		
	}
	fclose(p_stream);
	return queue;
}

