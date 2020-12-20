#ifndef FILE_SYSTEM_DOT_H
#define FILE_SYSTEM_DOT_H

#include "includes.h"
#include "Queue.h"
#include <stdio.h>
#include <string.h>

#define STATUS_CODE_FAILURE 1
#define STATUS_CODE_SUCCESS 0
#define MAX_NUMBER_LENGTH 9


/*
* Description - Extracts the priorty from file 
* Parameters - Pointer to file path of priorty
* Returns -pointer to Queue of bit positions
*/
Queue* extract_priorty_array(char* input_path_priorty, int amount_of_tasks);

/*
* Description - Makes sure a New, Clean output file is ready.
* Parameters - Pointer to file path of output file
* Returns -None
*/
 void clean_output_file(char* output_file);
#endif // !FILE_SYSTEM_DOT_H
