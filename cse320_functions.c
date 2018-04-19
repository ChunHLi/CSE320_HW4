#include "cse320_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <semaphore.h>
#include "csapp.h"
#include <errno.h>
#include <sys/types.h>


volatile int stor_addr_size = 0;
volatile int stor_files_size = 0;
volatile int INTERVAL = 1;


sem_t mutex_addr;
sem_t mutex_files;
sem_t mutex_timer;
sem_t mutex_clean;

void cse320_init(){
	Sem_init(&mutex_addr,0,1);
	Sem_init(&mutex_files,0,1);
	Sem_init(&mutex_timer,0,1);
	Sem_init(&mutex_clean,0,1);
	int a;
	for (a = 0; a < 25; a++){
		stor_addr[a].addr = NULL;
		stor_addr[a].ref_count = 0;
		stor_files[a].fptr = NULL;
		stor_files[a].filename = "";
		stor_files[a].ref_count = 0;
	}
}


void* cse320_malloc(size_t size){
	while (1){
		P(&mutex_addr);
		void* ptr;
		if (stor_addr_size >= 25){
			errno = ENOMEM;
			perror("Not enough memory");
			cse320_clean();
			V(&mutex_addr);
			exit(-1);
		} else {
			ptr = malloc(size);
			stor_addr[stor_addr_size].addr = ptr;
			stor_addr[stor_addr_size].ref_count = 1;   
			stor_addr_size++;
		}
		V(&mutex_addr);
		return ptr;
	}
}

void cse320_free(void* ptr){
	while (1){
        	P(&mutex_addr);
		int i;
		for (i = 0; i < stor_addr_size; i++){
			if (stor_addr[i].addr == ptr && stor_addr[i].ref_count > 0){
				free(stor_addr[i].addr);
				//stor_addr[i].addr = NULL;
				stor_addr[i].ref_count = 0;
				goto skip;
			} else if (stor_addr[i].addr == ptr && stor_addr[i].ref_count == 0){
				errno = EADDRNOTAVAIL;
				perror("Free: Double free attempt\n");
				V(&mutex_addr);
				cse320_clean();
				exit(-1);	
			} else {
			}
		}
		errno = EFAULT;
		perror("Free: Illegal address\n");
		V(&mutex_addr);
		exit(-1);
		skip:
		V(&mutex_addr);
		return;
	}
}

FILE* cse320_fopen(const char *restrict fileName, const char *restrict mode){
	while (1){
        	P(&mutex_files);
		FILE* fp;
		if (stor_files_size >= 25){
			errno = ENFILE;
			perror("Too many opened files\n");
			V(&mutex_files);
			cse320_clean();
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
			stor_files[stor_files_size].ref_count = 1;
			fp = fopen(fileName, mode);
			stor_files[stor_files_size].fptr=fp; 
			stor_files_size++;
		}
		skip:
		V(&mutex_files);
		return fp;
	}
}

int cse320_fclose(FILE *stream){
        while (1){
		P(&mutex_files);
		int i;
		for (i = 0; i < stor_files_size ;i++){
			if (stor_files[i].fptr == stream && stor_files[i].ref_count <= 0) {
				errno = EINVAL;
				perror("Close: Ref count is zero\n");
				V(&mutex_files);
				return -1;
			} else if (stor_files[i].fptr == stream && stor_files[i].ref_count > 0){
				stor_files[i].ref_count -= 1;
				if (stor_files[i].ref_count == 0){
					close(fileno(stor_files[i].fptr));
				}
				V(&mutex_files);
				return 0;
			} else {	
			}
		}
		errno = ENOENT;
		perror("Close: Illegal filename\n");
		V(&mutex_files);
		return -1;
	}
}

void cse320_clean(){
	while (1){
		P(&mutex_clean);
		int i;
		for (i = 0; i < stor_addr_size; i++){
			if (stor_addr[i].ref_count == 1){
				cse320_free(stor_addr[i].addr);
			}
		}
		//int j;
		//for (j = 0; j < stor_files_size; j++){
		//	while (stor_files[j].ref_count > 0){
		//		cse320_fclose(stor_files[j].fptr);
		//	}
		//}
		V(&mutex_clean);
		return;
	}
}

void handler(int sig){
	struct itimerval tout;
   	signal(SIGALRM,handler);
	pid_t pid;
	pid = waitpid(-1, NULL, 0);
	//if (errno != ECHILD){
	//	unix_error("waitpid error");
	//}
	tout.it_interval.tv_sec = 0;
	tout.it_interval.tv_usec = 0;
	P(&mutex_timer);
   	tout.it_value.tv_sec = INTERVAL;
	V(&mutex_timer);
	tout.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &tout,0);
}

pid_t cse320_fork(){
	pid_t pid = fork();
	if (pid > 0){
		struct itimerval tout;
        	tout.it_interval.tv_sec = 0;
        	tout.it_interval.tv_usec = 0;
		P(&mutex_timer);
        	tout.it_value.tv_sec = INTERVAL;
        	V(&mutex_timer);
		tout.it_value.tv_usec = 0;
		setitimer(ITIMER_REAL, &tout,0);
		signal(SIGALRM, handler);
	}
	return pid;
}

void cse320_settimer(int new_interval){
	P(&mutex_timer);
	INTERVAL = new_interval;
	V(&mutex_timer);
}
