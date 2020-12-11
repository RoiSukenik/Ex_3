#ifndef PRIME_NUMBER_CALCULATOR_DOT_H
#define PRIME_NUMBER_CALCULATOR_DOT_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STATUS_CODE_FAILURE 1
#define STATUS_CODE_SUCCESS 0


typedef struct node {
	int data;
	node* next;
}node;

/*
* Description - Initialize prime number array, heap based
* Parameters - None
* Returns - Pointer to a array struct
*/
int* initialize_prime_num_list();

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
* Description - Free prime number list
* Parameters - Pointer to number list
* Returns - set list pointer to null
*/
void free_list(node* p_prime_list);



#endif // !PRIME_NUMBER_CALCULATOR_DOT_H
