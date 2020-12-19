#include "filesystem.h"
#include <Windows.h>
#include "Queue.h"

FILE* open_file(char* file_path)
{
	errno_t retval;
	FILE* p_fs = NULL;
	retval = fopen_s(p_fs, file_path, 'r+');
	if (retval != 0) { printf("Failed to open file"); exit(retval); }
	return p_fs;
}

void close_file(FILE* p_file_stream)
{
	errno_t retval = fclose(p_file_stream);
	if (retval != 0) { printf("Failed to close file");	exit(retval); }
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
Queue* extract_priorty_array(char* input_path_priorty)
{
	Queue* queue = InitializeQueue();
	FILE* p_fs_prior = open_file(input_path_priorty);
	char* c = NULL , p_line = NULL;
	int* p_priorty_array = NULL;
	int amount_of_lines_file = amount_of_lines(p_fs_prior);
	for (int i = 0; i < amount_of_lines_file; i++)
	{
		while (c != '\r')
		{
			c = fgetc(p_fs_prior);
			sprintf_s(p_line, sizeof(char), "%c", c);
		}
		c = fgetc(p_fs_prior); //Leap over the \n
		char* ptr;
		int num;
		num = strtol(p_line, &ptr, 10);
		Push(queue, num);
	}
	close_file(p_fs_prior);

	return queue;
}

