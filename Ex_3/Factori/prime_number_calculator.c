#include "prime_number_calculator.h"

node *add_prime(node* p_prime_num_list , int num)
{
	int value = num;
	node *new_node = NULL;
	if (num > 0)
	{
		new_node = (node*)malloc(sizeof(node));
		if (NULL == new_node) {
			printf_s("Failed to allocate memory 2\n\n");
			exit(STATUS_CODE_FAILURE);
		}
		new_node->data = value;
		new_node->next = p_prime_num_list;
		p_prime_num_list = new_node;
	}
	return p_prime_num_list;
}

node *divid_number_add_2_list(int num)
{
	node* p_prime_list = NULL;
	while (num % 2 == 0)
	{
		p_prime_list = add_prime(p_prime_list,2);
		num = num / 2;
	}
	for (int i = 3; i <= sqrt(num); i = i + 2)
	{
		while (num % i == 0)
		{
			p_prime_list = add_prime(p_prime_list,i);
			num = num / i;
		}
	}
	if (num > 2) p_prime_list = add_prime(p_prime_list,num);

	return p_prime_list;
}
void print_list(node* p_prime_list)
{
	node* temp;
	while (p_prime_list != NULL)
	{
		temp = p_prime_list;
		p_prime_list = p_prime_list->next;
		int check = temp->data;
		printf(check);
	}
}
void free_list(node* p_prime_list)
{
	node* temp;
	while (p_prime_list != NULL)
	{
		temp = p_prime_list;
		p_prime_list = p_prime_list->next;
		int check = temp->data;
		free(temp);
	}
	printf_s("All memory allocations are freed!");
}
