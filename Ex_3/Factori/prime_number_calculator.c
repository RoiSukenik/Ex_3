#include "prime_number_calculator.h"

node* initialize_prime_num_list(void)
{
	node* p_prime_num_list = NULL;
	p_prime_num_list = (node*)malloc(sizeof(node));
	if (NULL == p_prime_num_list)
	{
		printf_s("Failed to allocate memory\n\n");
		exit(STATUS_CODE_FAILURE);
	}
	p_prime_num_list->next = NULL;
	return p_prime_num_list;
}

node* add_prime(node* p_prime_num_list , int num)
{
	int value = num;
	node* new_node = NULL;
	if (num > 0)
	{
		new_node = (node*)malloc(sizeof(node));
		if (NULL == new_node) {
			printf_s("Failed to allocate memory\n\n");
			exit(STATUS_CODE_FAILURE);
		}
		new_node->data = value;
		new_node->next = p_prime_num_list;
		p_prime_num_list = new_node;
	}
	return p_prime_num_list;
}

node* divid_number_add_2_list(int num)
{
	node* p_prime_list = initialize_prime_num_list();
	while (num % 2 == 0)
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
	node* temp ;
	while (p_prime_list->next != NULL)
	{
		temp = p_prime_list;
		p_prime_list = p_prime_list->next;
		free(temp);
	}
	free(p_prime_list);
	printf_s("All Memory Allocations Freed!\n\n");
	return STATUS_CODE_SUCCESS;
}
