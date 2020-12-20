#include "filesystem.h"



void clean_output_file(char* output_file)
{
	errno_t retval;
	FILE* p_stream;
	// Open file in write mode, this mode overwrites/creates this empty file
	retval = fopen_s(&p_stream, output_file, "w");
	if (0 != retval) 
	{ printf("Failed to open file.\n");	exit(retval); }
	retval = fclose(p_stream);
	if (0 != retval)
	{ printf("Failed to close file.\n");	exit(retval); }
}
int amount_of_lines(FILE* p_fs) {
	int amount_of_lines = 0;
	char c = NULL;
	char* p_line = NULL;

	p_line = (char*)malloc(sizeof(char));
	if (NULL == p_line) {
		printf_s("Failed to allocate memory\n\n");
		exit(STATUS_CODE_FAILURE);
	}

	while (c != EOF)
	{
		c = fgetc(p_fs);
		if (c == '\n') amount_of_lines++;
	}
	return amount_of_lines;
}
Queue* extract_priorty_array(char* input_path_priorty, int amount_of_tasks)
{
	Queue* queue = InitializeQueue();
	errno_t retval;
	FILE* p_stream;

	char line [MAX_NUMBER_LENGTH];
	char* p_line = NULL;
	char* left_token = NULL;
	char* ptr ;
	retval = fopen_s(&p_stream, input_path_priorty, "r");
	char* delim = "\n";
	if (0 != retval) { printf("Failed to open file.\n");	exit(STATUS_CODE_FAILURE); }
	for (int i = 0; i < amount_of_tasks; i++)
	{
		
		
		fgets(line,MAX_NUMBER_LENGTH,p_stream);
		rsize_t strmax = sizeof line;
		p_line = strtok_s(line, delim, &left_token);
		
		int num;
		num = strtol(p_line, &ptr, 10);
		Push(queue, num);
		
		
	}
	fclose(p_stream);
	return queue;
}

