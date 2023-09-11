# isPrime
Program IsPrime takes as argument the number of threads and start to read a list of numbers
Every number that reads is giving it to a thread which checking if number is Prime or not
#if you give 0 or negative is terminating

struct
	We are using a struct ThreadData_t for each thread:
		pthread_t *id        : the id of thread 
		int available        : 0 thread is not available and 1 is available
		int val              : is the number that thread has to check with isPrime function
		int work             : 0 means that thread dont have work and 1 has work
		int terminate_order  : 1 when main want the thread to terminate
		int terminate_flag   : 1 when want to notify main that is going to finish
		
Thread Management
	We are using an array of structs(threads) 
	array_init initializing the array

# quick_sort
The program quick_sort is reading an array, sorting it and print it
The sorting is done with the quick_sort algorithm where each recursion call is assigned to a thread

Struct QSData_t:
	pthread_t *id  : the thread id
	int low        : is the low pointer
	int high       : is the high pointer
	int terminated : 1 when is terminated

# pipe
Pipe.h:
  Usage:
		this library offer tools to create,use and destroy a pipe which follows the circural buffer model
		Uses an array (ring buffer) of chars
		
  Functions:
		pipe_t *pipe_init(pipe_t *pipe, int size): initialize the pipe
		void pipe_WriteDone(pipe_t *pipe): you call it when you have finish with the writing
		void pipe_destroy(pipe_t *pipe): you call it in order to destroy the pipe
	pipe_t:
		library use a data type named "pipe_t" which is a struct that includes:
			struct {
				char *RingBuffer   : the array of char, the pipe 
				int state          : if 1 the pipe is full, else 0 
				int size           : is the size of pipe
				char *pWriteData   : the data you want to write in the pipe
				char *pReadData    : the data that you read from the pipe
				int pwrite_state   : if 1 library complete's the write of data to the pipe, else 0
				int pread_state    : if 1 library complete's the reading of data from the pipe, else 0
				....
			}
   
Test.c:
	This program copies a file using the pipe.h 
	Step:
		1.Reads the size of pipe we want to use(in bytes)
		2.Read name of file to open
		3.Read name of copy file
		4.Initialize a pipe_t pointer
		5.Reads from file and write to the piep until  gets full
		6.Write from pipe to file until gets empty
		7.When the reading ends call the WriteDone()
		8. Destroy pipe
		9.Close the two files
		
