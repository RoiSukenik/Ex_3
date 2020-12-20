#ifndef PRIME_NUMBER_CALCULATOR_DOT_H
#define PRIME_NUMBER_CALCULATOR_DOT_H

#include "includes.h"

#define STATUS_CODE_FAILURE -1
#define STATUS_CODE_SUCCESS 0
#define STRING_OUTPUT_BUFFER 29
#define STRING_OUTPUT_NUMBER_BUFFER 5


#define size_of_buffer  10

typedef struct _node {
	int original_num;
	int data;
	struct _node* next;
}node;

/*
* Description - Initialize prime number array, heap based
* Parameters - None
* Returns - Pointer to a array struct
*/
node* initialize_prime_num_list(int num);

/*
* Description - realloc in prime num arr, add prime number, update pointer
* Parameters -  int to add
* Returns - Pointer to the updated number list
*/
node* add_prime(node* prime_num_arr , int num);

/*
* Description - Fill prime number list with the prime numbers
* Parameters -  int to divid
* Returns - Pointer to the updated number list
*/
node* divid_number_add_2_list(int num);

/*
* Description - print prime number list
* Parameters - Pointer to number list
* Returns - pointer to string
*/
char* print_prime_list(node* p_prime_list);






#endif // !PRIME_NUMBER_CALCULATOR_DOT_H
