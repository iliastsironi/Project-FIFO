#include <stdio.h>
#include <stdlib.h>
#include "pipe.h"


FILE *open(char *filename, int create){
	FILE *codition;
	//Creates file (create =1)
	if (create){
		codition = fopen(filename,"w");
		if (!codition){
			printf("\nWe cant create the file\n");
			exit(-1);
		}
		return(codition);
	}
	//Open an allready exist file (create =0)
	codition = fopen(filename,"r");
	if (!codition){
		printf("\nWe cant open the file\n");
		exit(-1);
	}
	return(codition);
}

int main(int argc, char *argv[]){
	char *RFName, *WFName, *BufferData;
	int size, fcheck,reach_end=0;
	pipe_t *info=NULL;
	FILE *rfd, *wfd;
	
	//Read pipe size
	printf("\nPlease give the size (in bytes) of pipe you want to use: ");
	scanf("%d", &size);
	//Open Read File 
	RFName = (char *)malloc(256);
	printf("\nPlease give the name of the file you want to copy: ");
	scanf("%s", RFName);
	rfd = open(RFName,0);
	//Open Write File
	WFName = (char *)malloc(256);
	printf("\nPlease give the name of the  file in whick you want to save the data: ");
	scanf("%s", WFName);
	wfd = open(WFName,1);
	
	//Main body
	info = pipe_init(info,size);
	while (!feof(rfd)){
		//Read from file and write to pipe
		while (!(info->state)){
			//Read from Read File
			BufferData = (char*)malloc(1);
			fcheck = fread(BufferData, 1, 1, rfd);
			if (!fcheck){
				reach_end++;
				break;
			}
			if(fcheck != 1){
				printf("\nfread has failed");
				exit(-1);
			}
			//Write Pipe
			*(info->pWriteData) = *BufferData;
			info->pwrite_state = 0;
			while (!(info->pwrite_state));
			free(BufferData);
		}
		//Read from pipe and write to file
		while (info->state){
			//read from pipe
			BufferData = (char*)malloc(1);
			info->pread_state = 0;
			
			//wait to read from pipe
			while(!(info->pread_state));
			
			//write to write file
			*BufferData = *(info->pReadData);
			fcheck = fwrite(BufferData, 1, 1, wfd);
			if(fcheck != 1){
				printf("\nfwrite has failed");
				exit(-1);
			}
			fflush(wfd);
			free(BufferData);
		}
		//Check if we reach the endof read file
		if (reach_end)
			break;
	}
	pipe_WriteDone(info);
	pipe_destroy(info);
	fclose(rfd);
	fclose(wfd);
	free(RFName);
	free(WFName);
	return(0);
}
