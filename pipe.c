#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "pipe.h"


int finish = 0;

pipe_t *pipe_init(pipe_t *pipe, int size){
	int error;
	
	//Initialize the struct
	pipe = (pipe_t *)malloc(sizeof(pipe_t));
	pipe->RingBuffer = (char *)malloc(sizeof(char)*size);
	pipe->head = 0;
	pipe->tail = 0;
	pipe->state =  0;
	pipe->size = size;
	pipe->pWriteData = (char *)malloc(sizeof(char));
	pipe->pReadData = (char *)malloc(sizeof(char));
	pipe->pwrite_state = 1;
	pipe->pread_state =1;
	pipe->wthread_id = (pthread_t *)malloc(sizeof(pthread_t));
	pipe->rthread_id = (pthread_t *)malloc(sizeof(pthread_t));
	
	//Create the threads
	error = pthread_create(pipe->wthread_id,NULL,pipe_write,(void*)pipe);
	if (error){
		printf("\nwrite thread not created, error: %d\n", error);
		free(pipe->wthread_id );
		exit(-1);
	}
	error = pthread_create(pipe->rthread_id,NULL,pipe_read,(void*)pipe);
	if (error){
		printf("\nread thread not created, error: %d\n", error);
		free(pipe->rthread_id );
		exit(-1);
	}
	return(pipe);
} 

void *pipe_write(void *info){
	int next_pos;
	pipe_t* pipe;
	pipe = (pipe_t*)info;
	
	while (1){
		while (pipe->state);
		if (!(pipe->pwrite_state)){
			//Check where to move the head pointer
			next_pos = pipe->head + 1;
			if (next_pos >= pipe->size){
				next_pos = 0;
			}
			// Circular buffer is full
			if (next_pos == pipe->tail){
				pipe->state = 1;
			}
			// Load data and move HeadPointer
			pipe->RingBuffer[pipe->head] = *(pipe->pWriteData);  
			pipe->head = next_pos;
		}
		pipe->pwrite_state = 1;
		if (finish){
			break;
		}
	}
	return(NULL);
}

void *pipe_read(void *info){
	int next_pos;
	pipe_t *pipe;
	pipe = (pipe_t*)info;
	
	while (1){
		if (!(pipe->pread_state)){
			// Circular buffer is empty
			if (pipe->head == pipe->tail) {
				//if WriteDone has called before
				if (finish){
					return(NULL);
				}
				pipe->state = 0;
			}
			//Check where to move the tail pointer
			next_pos = pipe->tail + 1;
			if (next_pos >= pipe->size){
				next_pos = 0;
			}
			//Read data and move the tail
			*(pipe->pReadData) = pipe->RingBuffer[pipe->tail];
			pipe->tail = next_pos;
			pipe->pread_state = 1;
		}
	}
	return(NULL);
}

void pipe_WriteDone(pipe_t *pipe){
	finish = 1;
}

void pipe_destroy(pipe_t *pipe){
	free(pipe->rthread_id );
	free(pipe->wthread_id );
	free(pipe->RingBuffer);
	free(pipe);
	return;
}
