#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h>

typedef struct QSData QSData_t;
typedef struct QSData{
	pthread_t *id;
	int low;
	int high;
	int terminated;  //If thread terminated is 1 else 0
}QSData;
int *array;

void swap(int *a, int* b) { 
	int t = *a; 
	*a = *b; 
	*b = t; 
} 

void print_array(int *arr,int size){
	int i;
	printf("\nThe sorted array is:\n[ ");
	for (i=0; i < size; i++){
		printf("%d ", arr[i]);
	}
	printf("]\n");
	return;
}

void *QuickSort(void  *QSinfo){ 
	int pi, pivot, i, j;
	QSData_t *info, *Curr_info;
	
	Curr_info = (QSData_t *)malloc(sizeof(QSData_t));
	Curr_info = (QSData_t *)QSinfo;
	
	if (Curr_info->low < Curr_info->high){ 
		//Partition
		pivot = array[Curr_info->high];
		i = (Curr_info->low) - 1;
		for (j = Curr_info->low; j <= Curr_info->high - 1; j++){
			if (array[j] < pivot){
				i++;
				swap(&array[i],&array[j]);
			}
		}
		swap(&array[i+1],&array[Curr_info->high]);
		pi = i+1;
		
		//Quick Sort Recursion 1
		info = (QSData_t *)malloc(sizeof(QSData_t));
		info->id = (pthread_t *)malloc(sizeof(pthread_t));
		info->low = Curr_info->low;
		info->high = pi-1;
		info->terminated = 0;
		
		pthread_create(info->id,NULL,QuickSort,(void *)info);
		while (!(info->terminated));
		free(info->id); 
		free(info);
		
		//Quick Sort Recursion 2
		info = (QSData_t *)malloc(sizeof(QSData_t));
		info->id = (pthread_t *)malloc(sizeof(pthread_t));
		info->low = pi+1;
		info->high = Curr_info->high;
		info->terminated = 0;
		
		pthread_create(info->id,NULL,QuickSort,(void *)info); 
		while (!(info->terminated));
		free(info->id);
		free(info);
	} 
	Curr_info->terminated = 1;
	return(NULL);
} 

//Program to test QuickSort 
int main(int argc, char *argv[]){
	int size, i;
	QSData_t *info;
	
	//Read the array
	printf("\nPlease give the size of array you want to sort: ");
	scanf("%d", &size);
	if (!size || size<0){
		return(0);
	}
	array = (int *)malloc(size * sizeof(int));
	printf("Please give the elements of array: \n");
	for (i=0; i < size; i++){
		scanf("%d",&array[i]);
	}
	
	//Call QuickSort
	info = (QSData_t *)malloc(sizeof(QSData_t));
	info->id = (pthread_t *)malloc(sizeof(pthread_t));
	info->low = 0;
	info->high = size-1;
	info->terminated = 0;
	
	pthread_create(info->id,NULL,QuickSort,(void*)info); 
	while (!(info->terminated));
	free(info->id);
	free(info);
	
	//Printf Final Array
	print_array(array,size);
	free(array);
	return 0; 
} 
