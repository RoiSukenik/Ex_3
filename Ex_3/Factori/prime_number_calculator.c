#include "prime_number_calculator.h"

node* initialize_prime_num_list()
{
	node* p_prime_num_arr = NULL;
	p_prime_num_arr = (node*)malloc(sizeof(node));
	if (NULL == p_prime_num_arr)
	{
		printf_s("Failed to allocate memory");
		exit(STATUS_CODE_FAILURE);
	}
	return p_prime_num_arr;
}

node* add_prime(node* prime_num_arr , int num)
{
	int value = 0;
	node* new_node = NULL;
	while (num > 0)
	{
		new_node = (node*)malloc(sizeof(node));
		if (NULL == new_node) {
			printf_s("Failed to allocate memory");
			exit(STATUS_CODE_FAILURE);
		}
		new_node->data = value;
		new_node->next = prime_num_arr;
	}
	return new_node;
}

node* divid_number_add_2_list(int num)
{
	node* p_prime_list = initialize_prime_num_list();
	while (num % 2 > 0)
	{
		p_prime_list = add_prime(p_prime_list ,2);
		num = num / 2;
	}
	for (int i = 3; i <= sqrt(num); i = i + 2)
	{
		while (num % i == 0)
		{
			p_prime_list = add_prime(p_prime_list , i);
			num = num / i;
		}
	}
	if (num > 2) p_prime_list = add_prime(p_prime_list , num);

	return p_prime_list;
}
void free_list(node* p_prime_list)
{
	node* temp;
	while (p_prime_list != NULL)
	{
		temp = p_prime_list;
		p_prime_list = p_prime_list->next;
		free(temp);
	}
}
