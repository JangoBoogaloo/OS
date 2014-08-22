#include <stdio.h>


void select_sort(int array[], int size)
{

}

void merge(int array1[], const int array2[], const int size)
{

}

void merge_sort(int array[], int size, int base_size)
{
	int mid = -1;

	if(size <= base_size)
	{
		select_sort(array, size);
		return;
	}

	mid = size/2;
	merge_sort(array+mid, size/2, base_size);
	merge_sort(array, size/2, base_size);
	merge(array, array+mid, size/2);
}

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	return 0;
}
