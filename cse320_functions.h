#ifndef CSE320_FUNCTIONS_H_
#define CSE320_FUNCTIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

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
struct files_in_use stor_files[25];

void cse320_init();
void* cse320_malloc(size_t size);
void cse320_free(void* ptr);
FILE *cse320_fopen(const char *restrict filename, const char *restrict mode);
int cse320_fclose(FILE *stream);
void cse320_clean();
int cse320_fork();
void cse320_settimer(int new_interval);

#endif // CSE320_FUNCTIONS_H_
