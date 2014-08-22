#include <stdio.h>
#include <assert.h>

static void print_array(int array[], const int size)
{
	int i=0;
	for(i=0; i<size; i++)
	{
		printf("%d\t", array[i]);
	}

	printf("\n");
}

static void select_sort(int array[], const int size)
{
	int i = -1;
	int j = -1;
	int temp = -1;
	int min_index = -1;
	assert(size >0);
	for(i=0; i<size; i++)
	{
		min_index = i;
		for(j=i+1; j<size; j++)
		{
			/* found a smaller index, record it */
			if(array[j] <array[min_index])
			{
				min_index = j;
			}
		}
		temp = array[i];
		array[i] = array[min_index];
		array[min_index] = temp;
	}
}

static void merge(int array1[], const int array2[], const int size)
{

}

static void merge_sort(int array[], int size, int base_size)
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
	int size = 10;
	int array[size];
	int i = -1;

	for(i=0; i<size; i++)
	{
		array[i] = size -i;
	}

	merge_sort(array, size, 100);
	print_array(array, size);

	(void) argc;
	(void) argv;
	return 0;
}
