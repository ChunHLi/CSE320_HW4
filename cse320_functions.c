#include "cse320_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void* cse320_malloc(size_t size){
	void* ptr;
	return ptr;
}

void cse320_free(void* ptr){
}

FILE *cse320_fopen(const char *restrict filename, const char *restrict mode){
	FILE* f;
	return f;
}

int cse320_fclose(FILE *stream){
	return 0;
}

void cse320_clean(){
}

int cse320_fork(){
	return 0;
}

int cse320_settimer(int which, const struct itimerval *new_value, struct itimerval *old_value){
	return 0;
}
