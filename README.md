# Homework #4
Do not cheat and good luck!

It will be great (and useful for you) if you will document your work in this README and write a sort of documentation for your homework. We may give few extra credits for good documentation.

**THIS IS THE DOCUMENTATION OF CSE320 HW4 by SHAWN LI**
**EACH FUNCTION HAS BEEN MADE WITH THE INTENTION OF FOLLOWING THE DOCUMENT PROVIDED BY THE PROFESSOR**

## Part 1 ##
Currently running "run X" in the pseudo terminal will block the main application and execute X. Any inputs to the main application will be **DELAYED** until the child process ends.

## Part 2 & 3 ##
General Contents of cse320_functions

```
struct addr_in_use* get_stor_addr(); //returns stor_addr USE FOR TESTING
struct files_in_use* get_stor_files(); //returns stor_files USE FOR TESTING
void* cse320_malloc(size_t size);
void cse320_free(void* ptr);
FILE *cse320_fopen(const char *restrict filename, const char *restrict mode);
int cse320_fclose(FILE *stream);
void cse320_clean();
int cse320_fork();
void cse320_settimer(int new_interval); //sets the global variable interval which is used in setitimer
```

## BEFORE TESTING CODE: ##
**1. INITIALIZE WITH THE FUNCTION cse320_init() IN YOUR MAIN FUNCTION WHICH INITIALIZES MUTEX_ADDR, MUTEX_FILES, MUTEX_INTERVAL, STOR_ADDR AND STOR_FILES**

**2. END MAIN FUNCTION USING cse320_clean();**
