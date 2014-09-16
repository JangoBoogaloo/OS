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

static void partition(int array[], const int low, const int piv,
											 const int high)
{

}

static void quick_sort(int array[], const int low, const int high,
											 const int base_size)
{
	int size = high - low +1;
	int piv = -1; 
	if(low < high)
	{
		if(size <= base_size)
		{
			select_sort(array, size);
			return;
		}

		piv = low + (high - low) / 2;
		partition(array, low, piv, high);

		quick_sort(array, low, piv, base_size);
		quick_sort(array, piv, high, base_size);
	}
}

int main(void)
{
	int size = 10;
	int array[size];
	int i = -1;

	for(i=0; i<size; i++)
	{
		array[i] = size -i;
	}

	quick_sort(array, 0, size-1, 10);
	print_array(array, size);

	return 0;
}
