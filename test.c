#include "cse320_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "csapp.h"

sem_t mutex_test;
FILE* test;


void print_stor_addr(){
        printf("\nstor_addr ref_count: \n");
        int k;
        for (k = 0; k < 25; k++){
                printf("stor_addr[%d] : %d\n", k, stor_addr[k].ref_count);
        }
        printf("Finished printing stor_addr ref_count\n");
}

void print_stor_files(){
	printf("\nstor_files ref_count: \n");
	int k;
	for (k = 0; k < 25; k++){
		printf("stor_files[%d] : %d\n", k, stor_files[k].ref_count);
	}
	printf("Finished printing stor_files ref_count\n");
}

void *test_0(void* vargp){
	void* tmp = cse320_malloc(4*sizeof(int));
	cse320_malloc(4*sizeof(int));
	cse320_malloc(4*sizeof(int));
	print_stor_addr();
	cse320_free(tmp);
	print_stor_addr();
	cse320_free(tmp);
	print_stor_addr();
}

void *test_1(void* vargp){
	cse320_malloc(4*sizeof(int));
	cse320_malloc(4*sizeof(int));
	cse320_malloc(4*sizeof(int));
	cse320_malloc(4*sizeof(int));
	cse320_malloc(4*sizeof(int));
	print_stor_addr();
}

void *test_2(void* vargp){
	cse320_malloc(4*sizeof(int));
	print_stor_addr();
}

void *test_3(void* vargp){
	test = cse320_fopen("text.txt","r");
	print_stor_files();	
}

void *test_4(void* vargp){
	test = cse320_fopen("text.txt","r");
	print_stor_files();
}

int main(int argc, char** argv){
	cse320_init();
	Sem_init(&mutex_test,0,1);
	int i = 0;
	printf("Starting test of HW4...\n");
	pthread_t tid_0;
	pthread_t tid_1;
	pthread_t tid_2;
	pthread_t tid_3;
	pthread_t tid_4;
	//pthread_create(&tid_0, NULL, test_0, &i);
	//pthread_create(&tid_1, NULL, test_1, &i);
	//pthread_create(&tid_2, NULL, test_2, &i);
	//pthread_create(&tid_3, NULL, test_3, &i);
	//pthread_create(&tid_4, NULL, test_4, &i);
	//pthread_join(tid_0,NULL);
	//pthread_join(tid_1,NULL);
	//pthread_join(tid_2,NULL);
	//pthread_join(tid_3,NULL);
	//pthread_join(tid_4,NULL);
	//cse320_clean();
	//print_stor_files();
	cse320_settimer(2);
	cse320_fork();
	cse320_fork();
	cse320_fork();
	while(1){}
}
