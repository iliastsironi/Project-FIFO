#ifndef _PIPE_H
#define _PIPE_H

typedef struct PipeData{
	char *RingBuffer;
	int head;
	int tail;
	int state;
	int size;
	char *pWriteData;
	char *pReadData;
	int pwrite_state;
	int pread_state;
	pthread_t *wthread_id;
	pthread_t *rthread_id;
}pipe_t;

pipe_t *pipe_init(pipe_t *pipe, int size);
void *pipe_write(void *info);
void *pipe_read(void *info);
void pipe_WriteDone(pipe_t *pipe);
void pipe_destroy(pipe_t *pipe);

#endif
