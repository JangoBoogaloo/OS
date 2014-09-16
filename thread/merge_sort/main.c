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

static void merge(int array1[], int array2[], const int size_1, const int size_2)
{
	int size_3 = size_1 + size_2;
	int array3[size_3];
	int index_1 = 0;
	int index_2 = 0; 
	int index_3 = 0;

	for(index_3 =0; index_3 < size_3; index_3++)
	{
		if(index_1 >= size_1)
		{
			array3[index_3] = array2[index_2];
			index_2++;
			continue;
		}

		if(index_2 >= size_2)
		{
			array3[index_3] = array1[index_1];
			index_1++;
			continue;
		}

		if(array1[index_1] < array2[index_2])
		{
			array3[index_3] = array1[index_1];
			index_1++;
		}
		else
		{
			array3[index_3] = array2[index_2];
			index_2++;
		}
	}

	for(index_3 = 0; index_3 < size_3; index_3++)
	{
		array1[index_3] = array3[index_3];
	}
}

static void merge_sort(int array[], int size, int base_size)
{
	int mid = -1;
	int size_1 = -1;
	int size_2 = -1;

	if(size <= base_size)
	{
		select_sort(array, size);
		return;
	}

	mid = size/2;
	size_1 = size/2;
	size_2 = size/2;

	if(0 != size%2)
	{
		size_2++;
	}

	merge_sort(array, size_1, base_size);
	merge_sort(array+mid, size_2, base_size);
	merge(array, array+mid, size_1, size_2);
}

int main(int argc, char **argv)
{
	int size = 100000;
	int array[size];
	int i = -1;

	for(i=0; i<size; i++)
	{
		array[i] = size -i;
	}

	merge_sort(array, size, 1000);
	print_array(array, size);

	(void) argc;
	(void) argv;
	return 0;
}
