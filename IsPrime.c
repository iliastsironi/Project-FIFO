#include <stdio.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct ThreadData ThreadData_t;
typedef struct ThreadData{
	pthread_t *id;
	int available;
	int val;
	int work;
	int terminate_order;
	int terminate_flag;
}ThreadData;

ThreadData_t *array;
int num =0;

int isPrime(int val) {
	int i, flag = 0;
	for (i = 2; i <= val / 2; ++i) {
		// condition for non-prime
		if (val % i == 0) {
			flag = 1;
			break;
		}
	}
	if (val == 1) {
		return(-1);
	}
	else {
		if (flag == 0)
			return(1);	//number is prime
		else
			return(0);	//number is not prime
	}
} 

void *array_init(ThreadData_t *arr,int num){
	int i;
	arr = (ThreadData_t *)malloc(num * sizeof(ThreadData_t));
	for(i=0; i < num; i++){
		arr[i].id = (pthread_t *)malloc(sizeof(pthread_t));
		arr[i].available = 1;
		arr[i].val = 0;
		arr[i].work = 0;
		arr[i].terminate_order = 0;
		arr[i].terminate_flag = 0;
	}
	return(arr);
}

void *routine(void *info1){
	int flag;
	ThreadData_t* info;
	info = (ThreadData_t*)info1;	//type casting the info in order to use it as we want and not as pthread_create needs
	
	while (1){
		if (info->terminate_order){
			break;
		}
		if (info->work){
			info->available = 0;
			flag = isPrime(info->val);
			if (flag == 1) {
				printf("%d is a prime number.\n", info->val);
			}
			else if (flag == 0) {
				printf("%d is not a prime number.\n", info->val);
			}
			else {
				printf("1 is neither prime nor composite.\n");	//number 1 is neither prime nor composite
			}
			info->available = 1;
			info->work= 0;
		}
	}
	info->terminate_flag = 1;
	free(info->id);
	info->id =NULL;
	return(NULL);
}

int main(int argc, char *argv[]){
	int number, counter,i = 0, j=0,FindAvailWorker;
	
	//Read number of threads and make the thread id array
	num = atoi(argv[1]);
	if (num <= 0){
		return(0);
	}
	array = array_init(array,num);
	
	//Main Thread
	while (1){
		while (j < num){
			scanf("%d",&number);
			if (number <= 0){
				//Wait all workers to became available
				i=0;
				counter=1;
				while (counter){
					counter = 0;
					for(i=0;i < j-1;i++){
						if (!(array[i].available)){
							counter++;
						}
					}
				}
				free(array);
				return(0);
			}
			array[j].work =1;
			array[j].val = number;
			if (pthread_create(array[j].id,NULL,routine,&array[j])){
				printf("\n%d thread not created\n", j);
				return(-1);
			}
			j++;
		}
		scanf("%d",&number);
		if (number <= 0){
			break;
		}
		//Searching for available worker 
		while (1){
			FindAvailWorker=0;
			for(i =0; i < num; i++){
				if (array[i].available){
					FindAvailWorker = 1;
					break;
				}
			}
			if (FindAvailWorker){
				break;
			}
		}
		//Set Work to the available worker
		array[i].work = 1;
		array[i].val = number;
	}
	//Wait all workers to became available
	i=0;
	counter=1;
	while (counter){
		counter = 0;
		for(i=0;i < num;i++){
			if (!(array[i].available)){
				counter++;
			}
		}
	}
	//Notify workers to terminate
	for(i=0;i < num;i++){
		array[i].terminate_order = 1;
	}
	//Wait for all workers to terminate
	counter = 1;
	while (counter){
		counter = 0;
		for(i=0;i < num;i++){
			if (!array[i].terminate_flag){
				counter++;
			}
		}
	}
	free(array);
	return(0);
}
