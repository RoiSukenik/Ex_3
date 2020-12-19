
#ifndef FILE_SYSTEM_DOT_H
#define FILE_SYSTEM_DOT_H

#include "includes.h"
#include "Queue.h"

#define STATUS_CODE_FAILURE -1
#define STATUS_CODE_SUCCESS 0

/*
* Description - Opens a Stream for file R/W
* Parameters - File Path
* Returns - Pointer to File 
*/
FILE* open_file(char* file_path);

/*
* Description - Closes File stream
* Parameters - Pointer to file
* Returns - none
*/
void close_file(FILE* p_file_stream);

/*
* Description - Extracts the amount of lines (tasks)
* Parameters - Pointer to file stream
* Returns -amount of lines
*/
int amount_of_lines(FILE* p_fs);

/*
* Description - Extracts the priorty from file 
* Parameters - Pointer to file path of priorty
* Returns -pointer to Queue of bit positions
*/
 Queue* extract_priorty_array(char* input_path_priorty);


 void clean_output_file(char* output_file);
#endif // !FILE_SYSTEM_DOT_H
