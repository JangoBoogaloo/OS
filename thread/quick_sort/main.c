#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

static void print_array(int array[], const int size)
{
	int i=0;
	for(i=0; i<size; i++)
	{
		printf("%d\t", array[i]);
	}

	printf("\n");
}

static int get_max_index(int array[], const int index_1, const int index_2)
{
	if(array[index_1] < array[index_2])
	{
		return index_2;
	}
	return index_1;
}

static int get_min_index(int array[], const int index_1, const int index_2)
{
	if(array[index_1] <= array[index_2])
	{
		return index_1;
	}
	return index_2;
}

static void swap(int array[], const int index_1, const int index_2)
{
	int temp = -1;
	temp = array[index_1];
	array[index_1] = array[index_2];
	array[index_2] = temp;
}

static int get_mid_index(int array[], const int low, const int piv,
												 const int high)
{
	int min = -1;
	int max = -1;
	int mid = -1;
	int sum = low+high+piv;
	min = get_min_index(array, low, piv);
	min = get_min_index(array, min, high);
	
	max = get_max_index(array, low, piv);
	max = get_max_index(array, max, high);
	mid = sum - min- max;
	
	return mid;
}

static int partition(int array[], const int low, const int piv,
										 const int high)
{
	int i= 0;
	int store_i = 0;
	int piv_i = get_mid_index(array, low, piv, high);

	swap(array, piv_i, high);
	for(i = 0; i < high; i++)
	{
		if(array[i] < array[high])
		{
			swap(array, i, store_i);
			store_i++;
		}

	}
	swap(array, store_i, high);
	return store_i;
}

static void quick_sort(int array[], const int low, const int high)
{
	int piv = -1; 
	if(low < high)
	{
		piv = low + (high - low) / 2;
		piv = partition(array, low, piv, high);
		quick_sort(array, low, piv-1);
		quick_sort(array, piv+1, high);
	}
}

int main(void)
{
	int size = 100000;
	int array[size];
	int i = -1;

	for(i=0; i<size; i++)
	{
		array[i] = size -i;
	}

	quick_sort(array, 0, size-1);
	print_array(array, size);

	return 0;
}
