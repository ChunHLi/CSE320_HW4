#include "cse320_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <semaphore.h>
#include "csapp.h"
#include <errno.h>

struct addr_in_use{
	void* addr;
	int ref_count;
};

struct files_in_use{
	FILE* fptr;
	const char* filename;
	int ref_count;
};

struct addr_in_use stor_addr[25];
struct files_in_use stor_files[25] = {
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0},
					{NULL,"",0}
				     };
volatile int stor_addr_size = 0;
volatile int stor_files_size = 0;

void* cse320_malloc(size_t size){
	sem_t mutex;
	Sem_init(&mutex,0,1);
	P(&mutex);
	void* ptr;
	if (stor_addr_size >= 25){
		printf("Not enough memory");
		V(&mutex);
		exit(-1);
	} else {
		ptr = malloc(size);
		stor_addr[stor_addr_size].addr = ptr;
		stor_addr[stor_addr_size].ref_count = 1;   
		stor_addr_size++;
	}
	V(&mutex);
	return ptr;
}

void cse320_free(void* ptr){
	sem_t mutex;
        Sem_init(&mutex,0,1);
        P(&mutex);
	int i;
	for (i = 0; i < stor_addr_size; i++){
		if (stor_addr[i].addr == ptr && stor_addr[i].ref_count > 0){
			stor_addr[i].ref_count = 0;
			goto skip;
		} else if (stor_addr[i].addr == ptr && stor_addr[i].ref_count == 0){
			errno = EADDRNOTAVAIL;
			printf("Free: Double free attempt\n");
			V(&mutex);
			exit(-1);	
		} else {
		}
	}
	errno = EFAULT;
	printf("Free: Illegal address\n");
	V(&mutex);
	exit(-1);
	skip:
	V(&mutex);
}

FILE *cse320_fopen(const char *restrict fileName, const char *restrict mode){
	sem_t mutex;
        Sem_init(&mutex,0,1);
        P(&mutex);
	FILE* fp;
	if (stor_files_size >= 25){
		printf("Too many opened files");
		V(&mutex);
		exit(-1);
	} else {
		int i;	
		for (i = 0; i < stor_files_size;i++){
			if (strcmp(fileName,stor_files[i].filename)==0){
				fp = stor_files[i].fptr;
				stor_files[i].ref_count += 1;
				goto skip;
			}
		}
		stor_files[stor_files_size].filename = fileName;
		stor_files[stor_files_size].ref_count += 1;
		stor_files_size++;
		fp = fopen(fileName, mode); 
	}
	skip:
	V(&mutex);
	return fp;
}

int cse320_fclose(FILE *stream){
	sem_t mutex;
        Sem_init(&mutex,0,1);
        P(&mutex);
	int i;
	for (i = 0; i < stor_files_size ;i++){
		if (stor_files[i].fptr == stream && stor_files[i].ref_count <= 0) {
			errno = EINVAL;
			printf("Close: Ref count is zero\n");
			V(&mutex);
			exit(-1);
		} else if (stor_files[i].fptr == stream && stor_files[i].ref_count > 0){
			stor_files[i].ref_count -= 1;
			if (stor_files[i].ref_count == 0){
				close(fileno(stor_files[i].fptr));
			}
			V(&mutex);
			return 0;
		} else {	
		}
	}
	errno = ENOENT;
	printf("Close: Illegal filename\n");
	V(&mutex);
	exit(-1);
	return -1;
}

void cse320_clean(){
}

int cse320_fork(){
	return 0;
}

int cse320_settimer(int which, const struct itimerval *new_value, struct itimerval *old_value){
	return 0;
}
