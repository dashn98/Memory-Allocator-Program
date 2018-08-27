#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mymalloc.h"

// Don't change or remove these constants.
#define MINIMUM_ALLOCATION  16
#define SIZE_MULTIPLE       8

typedef struct Node{
	unsigned int size;
	unsigned int flag;
	struct Node* next;
	struct Node* prev;
}Node;

#define FREE 0
#define USED 1

static Node* head = NULL;
static Node* tail = NULL;



unsigned int round_up_size(unsigned int data_size)
{
	if(data_size == 0)
		return 0;
	else if(data_size < MINIMUM_ALLOCATION)
		return MINIMUM_ALLOCATION;
	else
		return (data_size + (SIZE_MULTIPLE - 1)) & ~(SIZE_MULTIPLE - 1);
}

Node* split(Node* node, unsigned int size)
{
	Node* second_node = (void*)node + size + sizeof(Node);
	second_node->prev = node;
	second_node->next = node->next;
	second_node->flag = FREE;
	second_node->size = node->size - size - sizeof(Node);
	second_node->next->prev = second_node;

	node->next = second_node;
	node->flag = USED;
	node->size = size;
	return node;
} 

Node* create_node(unsigned int size)
{
	Node* new_node = sbrk(size);
	new_node->size = size-sizeof(Node);
	new_node->flag = USED;
	new_node->prev = tail;
	new_node->next = NULL;
	tail->next = new_node;
	tail = new_node;
	return new_node;

}

Node* find_block(unsigned int size)
{
	Node* best_fit = NULL; //Node pointer for the node that will be the best fit
	Node* block = NULL; //Current Node
	
	for(block = head; block != NULL; block = block->next)
	{
		if(block->size >= size && block->flag == FREE)
		{
			if(best_fit == NULL)
			{
				best_fit = block;
			}
			else if( block->size < best_fit->size)
			{
				best_fit = block;
			}
		}
	}
	if(best_fit == NULL)
	{
		unsigned int the_size = size + sizeof(Node);
		best_fit = create_node(the_size);
	}
	else if(best_fit->size != size)
	{
		best_fit = split(best_fit, size);
	}
	else
	{
		best_fit->flag = USED;
	}
	return best_fit;
}

void* my_malloc(unsigned int size)
{
	Node* node = NULL;
	if(size == 0)
		return NULL;

	size = round_up_size(size);

	// ------- Don't remove anything above this line. -------

	if(head == NULL)
	{
		unsigned int the_size = size + sizeof(Node);
		head = sbrk(the_size);
		head->flag = USED;
		head->prev = NULL;
		head->next = NULL;
		head->size = size;
		tail = head;
		return ((void*)head) + sizeof(Node);
	}
	else
	{
		node = find_block(size);
		return ((void*)node) + sizeof(Node);

	}

	return NULL;
}

Node* join_nodes(Node* left_node, Node* right_node)
{
	left_node->next = right_node->next;
	left_node->size = left_node->size + right_node->size + sizeof(Node);

	if(right_node->next != NULL)
	{
		right_node->next->prev = left_node;
	}
	return left_node;
}

Node* coalesce(Node* node)
{
	
	if(node->next != NULL)
	{
		if(node->next->flag == FREE)
		{
			if(node == head)
			{
				if(node->next == tail)
				{
					node = join_nodes(node, node->next);
					tail = node;
					head = node;
				}
				else
				{
					node = join_nodes(node, node->next);
					head = node;
					
				}
			}

				
			else
			{
				if(node->next == tail)
				{
					
					node = join_nodes(node, node->next);
					tail = node;
					
				}
				else
				{
					
					node = join_nodes(node, node->next);
				
				}
			}
		}
	}


	if(node->prev != NULL)
	{
		if(node->prev->flag == FREE)
		{
			if(node->prev == head)
			{
				if(node == tail)
				{
					node = join_nodes(node->prev, node);
					tail = node;
					head = node;
				}
				else
				{
					node = join_nodes(node->prev, node);
					head = node;
				}
			}	
			else
			{
				if(node == tail)
				{
					node = join_nodes(node->prev, node);
					tail = node;
				}
				else
				{
					
					node = join_nodes(node->prev, node);
				}
			}
		}
	}
	return node;
}

void my_free(void* ptr)
{
	if(ptr == NULL)
		return;

	Node* node_ptr = ptr - sizeof(Node);
	node_ptr->flag = FREE;
	node_ptr = coalesce(node_ptr);

	if(node_ptr == tail)
	{
		if(node_ptr == head)
		{
			sbrk(-(sizeof(Node)+ node_ptr->size));
			head = NULL;
			tail = NULL;
		}
		else
		{
			tail = tail->prev;
			tail->next = NULL;
			sbrk(-(sizeof(Node)+ node_ptr->size));
		}
		
	}

}




