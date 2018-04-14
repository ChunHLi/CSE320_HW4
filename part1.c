#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "cse320_functions.h"

int loop(int argc, char* argv[], char** envp){
	
	if (argc != 1) {
		printf("Invalid number of arguments for part1\n");
	} else {
		int status = 1;	
		char str[255];
		printf("> Type 'help' for a list of commands.\n");
		
		do {
			char** args;
			printf("> ");
			fgets(str,255,stdin);
			char* p = strchr(str,'\n');
                        if (p != NULL){
                        	str[p-str]='\0';
                        }
			if (strcmp(str,"") == 0){
			
			} else if (strcmp(str,"exit") == 0){ 
				status = 0; 
			} else if (strcmp(str,"help") == 0){
				printf("- exit: exit this application\n");
				printf("- run X f1 f2 ... f5: execute application X with up to 5 arguments\n");
			} else {
				char* token;
				int i = 0;
				for (token=strtok(str," "); token != NULL; token=strtok(NULL, " ")){
					args[i] = strdup(token);
					i += 1;
				}
				args[i] = NULL;		
				if (strcmp(args[0],"run") == 0){
					if (i < 2){
						printf("Invalid number of arguments\n");
					}
					pid_t wpid;
					pid_t pid = fork();
					int pidstatus;
					if (pid == 0){
						if (execvp(args[1],args+1) == -1){
							perror("Unsuccessful Application\n");
						}
						exit(1);
					} else if (pid < 0) {
						perror("Error Forking\n");
					} else {
						do {
							wpid = waitpid(pid, &pidstatus, WUNTRACED);
						} while (!WIFEXITED(pidstatus) && !WIFSIGNALED(pidstatus));
					}
				} else {
					printf("Invalid command: %s not recognized\n", args[0]);
				}

				i -= 1;
				while (i >= 0){
					free(args[i]);
					i -= 1;	
				}	
			}
		} while (status);
	}
}

int main(int argc, char* argv[], char** envp){
	loop(argc, argv, envp);
	return 1;
}
