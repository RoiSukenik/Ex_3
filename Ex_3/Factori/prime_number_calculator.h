#ifndef PRIME_NUMBER_CALCULATOR_DOT_H
#define PRIME_NUMBER_CALCULATOR_DOT_H

/*
* Description - Initialize prime number array, heap based
* Parameters - None
* Returns - Pointer to a array struct
*/
int* initialize_prime_num_array();

/*
* Description - Fill prime number array with the prime numbers
* Parameters - Pointer to number array
* Returns - Pointer to the updated number array
*/
int* divid_number(int* number);


/*
* Description - Free prime number array
* Parameters - Pointer to number array
* Returns - set array pointer to null
*/
void free_array(int* number);



#endif // !PRIME_NUMBER_CALCULATOR_DOT_H
