#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "cse320_functions.h"
#include <errno.h>
#include <sys/time.h>
#include "csapp.h"
sigset_t mask;

void handleSIGHUP(int sig){
	pid_t pid;
        while ((pid = waitpid(-1, NULL, 0)) > 0){
        }
        if (errno != ECHILD){
                unix_error("waitpid error");
	}
}

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
				printf("\n- exit: exit this application\n");
				printf("- help: list commands available\n");
				printf("- run X f1 f2 ... f5: execute application X with up to 5 arguments\n\n");
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
					} else {
						pid_t wpid;
                                                sigemptyset(&mask);
						sigaddset(&mask,SIGINT);
						sigaddset(&mask,SIGQUIT);
						signal(SIGHUP,handleSIGHUP);
						signal(SIGINT, SIG_IGN);
						signal(SIGQUIT, SIG_IGN);
						pid_t pid = fork();
						sigprocmask(SIG_BLOCK, &mask, NULL);
						int pidstatus;
						if (pid == 0){
							sigprocmask(SIG_UNBLOCK, &mask, NULL);
							if (execvp(args[1],args+1) == -1){
								perror("Unsuccessful Application\n");
							}
							exit(1);
						} else if (pid < 0) {
							sigprocmask(SIG_UNBLOCK, &mask, NULL);
							perror("Error Forking\n");
						} else {
							do {
								wpid = waitpid(pid, &pidstatus, WUNTRACED);
							} while (!WIFEXITED(pidstatus) && !WIFSIGNALED(pidstatus));
							sigprocmask(SIG_UNBLOCK, &mask, NULL);
							signal(SIGINT, SIG_DFL);
							signal(SIGQUIT, SIG_DFL);
						}
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
