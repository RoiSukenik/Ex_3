#include "prime_number_calculator.h"

node* initialize_prime_num_list(int num)
{
	node* p_prime_num_list = NULL;
	p_prime_num_list = (node*)malloc(sizeof(node));
	if (NULL == p_prime_num_list)
	{
		printf_s("Failed to allocate memory\n\n");
		exit(STATUS_CODE_FAILURE);
	}
	p_prime_num_list->data = NULL;
	p_prime_num_list->original_num = num;
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
		new_node->original_num = p_prime_num_list->original_num;
		new_node->next = p_prime_num_list;
		p_prime_num_list = new_node;
	}
	return p_prime_num_list;
}

node* divid_number_add_2_list(int num)
{
	node* p_prime_list = initialize_prime_num_list(num);
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

void sort_list(node* list)
{
	node* temp = list;

	while (temp)
	{
		node* min = temp;
		node* r = temp->next;
		while (r) {
			if (min->data > r->data)
			{
				min = r;
			}
			r = r->next;
		}
		int x = temp->data;
		temp->data = min->data;
		min->data = x;
		temp = temp->next;
	}
}

char* print_prime_list(node* p_prime_list) {
	sort_list(p_prime_list);
	char* p_output_string=NULL;
	p_output_string = (char*)malloc(sizeof(char));
	if (NULL == p_output_string) {
		printf_s("Failed to allocate memory\n\n");
		exit(STATUS_CODE_FAILURE);
	}
	if (sprintf_s(p_output_string, sizeof(int)+STRING_OUTPUT_BUFFER, "The Prime Factor of %d are: ", p_prime_list->original_num) == STATUS_CODE_FAILURE)
	{
			printf_s("Failed to Write Formatted Data to String \n\n");
			exit(STATUS_CODE_FAILURE);
	}
	
	while (p_prime_list->next->next != NULL)
	{
		int size_p_string = strlen(p_output_string);//FIXME change the 10
		if (sprintf_s(p_output_string, size_p_string+size_of_buffer+sizeof(int), "%s%d, ",p_output_string, p_prime_list->next->data) == STATUS_CODE_FAILURE)
		{
			printf_s("Failed to Write Formatted Data to String \n\n");
			exit(STATUS_CODE_FAILURE);
		}
		
		p_prime_list = p_prime_list->next;
	}
	int size_p_string = strlen(p_output_string);
	
	if (sprintf_s(p_output_string, size_p_string + size_of_buffer + sizeof(int),
		"%s%d\r\n", p_output_string, p_prime_list->next->data) == STATUS_CODE_FAILURE)
	{
		printf_s("Failed to Write Formatted Data to String \n\n");
		exit(STATUS_CODE_FAILURE);
	}
	
	return p_output_string;

}

void free_list(node* p_prime_list)
{
	node* temp;

	while (!p_prime_list->next != NULL)
	{
		temp = p_prime_list;
		p_prime_list = p_prime_list->next;
		free(temp);
	}

	printf_s("All Memory Allocations Freed!\n\n");
}