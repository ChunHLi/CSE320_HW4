#ifndef CSE320_FUNCTIONS_H_
#define CSE320_FUNCTIONS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void* cse320_malloc(size_t size);
void cse320_free(void* ptr);
FILE *cse320_fopen(const char *restrict filename, const char *restrict mode);
int cse320_fclose(FILE *stream);
void cse320_clean();
int cse320_fork();
int cse320_settimer(int which, const struct itimerval *new_value, struct itimerval *old_value);

#endif // CSE320_FUNCTIONS_H_
