#include <stdio.h>
#include <unistd.h>

#include "mymalloc.h"

int main()
{
	// You can use sbrk(0) to get the current position of the break.
	// This is nice for testing cause you can see if the heap is the same size
	// before and after your tests, like here.
	//printf("Main");
	void* heap_at_start = sbrk(0);
	//printf("about to malloc 100\n");
	void* block = my_malloc(100);
	//printf("about to malloc 50\n");
	void* block2 = my_malloc(50);
	void* block3 = my_malloc(20);

	 //color it blue...
	my_free(block);
	my_free(block2);
	my_free(block3);
	void* block4 = my_malloc(30); // this pointer SHOULD be the same as block
	my_free(block4);
	void* block5 = my_malloc(10);
	void* block6 = my_malloc(900);
	my_free(block5);
	my_free(block6);
	void* block7 = my_malloc(8);
	void* block8 = my_malloc(15);
	my_free(block7);
	my_free(block8);
	void* block9 = my_malloc(1);
	void* block10 = my_malloc(38);
	void* block11 = my_malloc(324);
	void* block12 = my_malloc(29);
	my_free(block9);
	my_free(block10);
	my_free(block11);
	my_free(block12);
	

	void* heap_at_end = sbrk(0);
	unsigned int heap_size_diff = (unsigned int)(heap_at_end - heap_at_start);

	if(heap_size_diff)
		printf("Hmm, the heap got bigger by %u (0x%X) bytes...\n", heap_size_diff, heap_size_diff);

	

	// ADD MORE STUFF HERE.

	return 0;
}
