#include "prime_number_calculator.h"

node* initialize_prime_num_list()
{
	node* p_prime_num_list = NULL;
	p_prime_num_list = (node*)malloc(sizeof(node));
	if (NULL == p_prime_num_list)
	{
		printf_s("Failed to allocate memory\n\n");
		return NULL;
	}
	p_prime_num_list->next = NULL;
	p_prime_num_list->first = true;
	return p_prime_num_list;
}

node* add_prime(node* p_prime_num_list , int num)
{
	int value = num;
	node* new_node = NULL;
	if (p_prime_num_list->first == true && num>0)
	{
		p_prime_num_list->data = value;
		p_prime_num_list->first = false;
		return p_prime_num_list;
	}
	if (num > 0)
	{
		new_node = (node*)malloc(sizeof(node));
		if (NULL == new_node) {
			printf_s("Failed to allocate memory\n\n");
			return NULL;
		}
		new_node->first = false;
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
	if (p_prime_list == NULL) { return NULL; }
	p_prime_list->original_num = num;

	while (num % 2 == 0)
	{
		p_prime_list = add_prime(p_prime_list, 2);
		if (p_prime_list == NULL) { return NULL; }
		num = num / 2;

	}
	for (int i = 3; i <= sqrt(num); i = i + 2)
	{
		while (num % i == 0)
		{
			p_prime_list = add_prime(p_prime_list , i);
			if (p_prime_list == NULL) { return NULL; }
			num = num / i;
		}
	}
	if (num > 2 ) {p_prime_list = add_prime(p_prime_list, num); }
	if (p_prime_list == NULL) { return NULL; }

	return p_prime_list;
}

void sort_list(node* list)
{
	node* temp = list;

	while (temp != NULL)
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


	size_t needed = snprintf(NULL, 0, "The Prime Factor of %d are: ",p_prime_list->original_num) + 1;
	char* buffer = malloc(needed);
	if (NULL == buffer) {
		printf_s("Failed to allocate memory\n\n");
		exit(STATUS_CODE_FAILURE);
	}
	sprintf_s(buffer,needed, "The Prime Factor of %d are: ", p_prime_list->original_num);
	node* temp = p_prime_list;
	while (temp->next != NULL )
	{

		needed = snprintf(NULL, 0, "%s%d, ", buffer, p_prime_list->data)+1;
		buffer = realloc(buffer, needed);
		sprintf_s(buffer, needed, "%s%d, ", buffer, p_prime_list->data);
	
		temp = temp->next;
	}
	needed = snprintf(NULL, 0, "%s%d\r\n", buffer, temp->data) + 1;
	buffer = realloc(buffer, needed);
	sprintf_s(buffer, needed, "%s%d\r\n", buffer, temp->data);
	free_list(p_prime_list);
	return buffer;
}

void free_list(node* p_prime_list)
{
	node* temp =NULL;

	while (p_prime_list != NULL)
	{
		temp = p_prime_list;
		p_prime_list= temp->next;
		free(temp);
		

	}

	printf_s("All Memory Allocations Freed!\n\n");
}