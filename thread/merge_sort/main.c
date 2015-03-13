#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

struct MERGE_PARAM
{
	unsigned long *array;
	unsigned long size;
	unsigned long base_size;
};

static void print_array(unsigned long *array, const unsigned long size)
{
	unsigned long i=0;
	for(i=0; i<size; i++)
	{
		printf("%lu \t", array[i]);
	}
	printf("\n");
}

static void select_sort(unsigned long *array, const unsigned long size)
{
	unsigned long i = -1;
	unsigned long j = -1;
	unsigned long temp = -1;
	unsigned long min_index = -1;
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

static void merge(unsigned long *array1, unsigned long *array2, const unsigned long size_1, const unsigned long size_2)
{
	unsigned long size_3 = size_1 + size_2;
	unsigned long *array3 = malloc(sizeof(unsigned long)*size_3);
	unsigned long index_1 = 0;
	unsigned long index_2 = 0; 
	unsigned long index_3 = 0;

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

	free(array3);
}

static void *merge_sort(void *param)
//static void merge_sort(void *param)
{
	struct MERGE_PARAM* merge_param = (struct MERGE_PARAM*)param;

	unsigned long *array = merge_param->array;
	unsigned long size = merge_param->size;
	unsigned long base_size = merge_param->base_size;
	unsigned long mid = -1;
	unsigned long size_1 = -1;
	unsigned long size_2 = -1;

	if(size <= base_size)
	{
		select_sort(array, size);
		pthread_exit(0);
		//return;
	}

	mid = size/2;
	size_1 = size/2;
	size_2 = size/2;

	if(0 != size%2)
	{
		size_2++;
	}

	pthread_t thread1 = {0};
	pthread_t thread2 = {0};

	struct MERGE_PARAM param1 = {0};
	struct MERGE_PARAM param2 = {0};

	param1.array = array;
	param1.size = size_1;
	param1.base_size = base_size;

	param2.array = array+size_1;
	param2.size = size_2;
	param2.base_size = base_size;

	/*
	*/
	pthread_create(&thread1, NULL, merge_sort, &param1);
	pthread_create(&thread2, NULL, merge_sort, &param2);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	/*	
	merge_sort(&param1);
	merge_sort(&param1);
	*/

	merge(array, array+mid, size_1, size_2);
	pthread_exit(0);
}

int main(int argc, char **argv)
{
	unsigned long size = 1000000;
	unsigned long *array = malloc(sizeof(unsigned long)*size);
	unsigned long i = -1;

	for(i=0; i<size; i++)
	{
		array[i] = size -i;
	}

	pthread_t sort_thread = {0};
	struct MERGE_PARAM param = {0};

	param.array = array;
	param.size = size;
	param.base_size = 1000;
	struct timeval te;
	gettimeofday(&te, NULL);
	long long start_time = te.tv_sec*1000LL + te.tv_usec/1000;
	pthread_create(&sort_thread, NULL, merge_sort, &param);
	pthread_join(sort_thread, NULL);
	/*
	merge_sort(&param);
	*/
	gettimeofday(&te, NULL);
	long long end_time = te.tv_sec*1000LL + te.tv_usec/1000;
	long long duration = end_time - start_time;
	print_array(array, size);
	printf("\n\n%lld\n", duration);
	
	(void) argc;
	(void) argv;
	return 0;
}
