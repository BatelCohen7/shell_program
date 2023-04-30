#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <dirent.h>

#define TRUE 1
#define MAX_CMD_SIZE 1024
#define MAX_ARGS_SIZE 10

void handle_sigint(int sig) {
	if(sig == 2){
		int pid = fork();
		if(pid == -1){
			perror("fork function filed\n");  // handle error condition
        	exit(EXIT_FAILURE);
		}
		if(pid == 0){
			// int result = kill(getppid(), SIGSTOP);
			// if (result == -1) {
        	// 	perror("kill function filed\n");
        	// 	exit(EXIT_FAILURE);
    		// }
			char flag;
			while(TRUE){
				printf("press 1 to continue!\n");
				scanf("%c", &flag);
				if (flag == '1'){
					exit(EXIT_SUCCESS);
					// int result = kill(getppid(), SIGCONT);
					// if (result == -1){
        			// 	perror("kill function filed\n");
        			// 	exit(EXIT_FAILURE);
    				// }
				}
			}
		}else{
			waitpid(pid, NULL, 0);
		}
	}
	return;
}

void execute_pipe(char *commands[][MAX_ARGS_SIZE], int n) {
    int i;
    int pipefds[2 * (n - 1)];

    for (i = 0; i < (n - 1); i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("pipe function filed\n");
            exit(EXIT_FAILURE);
        }
    }

    int pid;
    int status;

    for (i = 0; i < n; i++) {
        pid = fork();
        if (pid == -1) {
            perror("fork function filed\n");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            if (i != 0) {
                if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) < 0) {
                    perror("dup2 function filed\n");
                    exit(EXIT_FAILURE);
                }
            }
            if (i != n - 1) {
                if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0) {
                    perror("dup2 function filed\n");
                    exit(EXIT_FAILURE);
                }
            }
            for (int j = 0; j < 2 * (n - 1); j++) {
                close(pipefds[j]);
            }

            if (execvp(commands[i][0], commands[i]) < 0) {
                perror(commands[i][0]);
                exit(EXIT_FAILURE);
            }
        }
    }
    for (int j = 0; j < 2 * (n - 1); j++) {
        close(pipefds[j]);
    }
    for (int j = 0; j < n; j++) {
        wait(&status);
    }
}

int main(){

	printf("--------------Welcome to stshell--------------\n");

    char cmd[MAX_CMD_SIZE] = "";
	const char *redirect_str1 = ">>";
	const char *redirect_str2 = "<<";

	struct sigaction sa;
	sa.sa_handler = handle_sigint;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGINT, &sa, NULL) == -1) {
		perror("sigaction");
		exit(EXIT_FAILURE);
		}

    while (TRUE){
		printf("\ncommand: ");
        fgets(cmd, MAX_CMD_SIZE, stdin);
        cmd[strlen(cmd) - 1] = '\0';

		if(strcmp(cmd, "exit") == 0){
			printf("\n--------------close to stshell--------------\n");
			exit(EXIT_SUCCESS);
		}
        else if (strchr(cmd, '|') != NULL){
    		char *commands[MAX_CMD_SIZE][MAX_ARGS_SIZE];
    		int num_commands = 0;
    		char *token = strtok(cmd, "|");
    		while (token != NULL) {
        		char *cmd_token = strtok(token, " ");
        		int num_args = 0;
        		while (cmd_token != NULL) {
            		commands[num_commands][num_args] = cmd_token;
            		cmd_token = strtok(NULL, " ");
            		num_args++;
        		}
        		commands[num_commands][num_args] = NULL;
        		num_commands++;
        		token = strtok(NULL, "|");
    		}
			execute_pipe(commands, num_commands);
        }
		else if ((strchr(cmd, '>') != NULL) || (strchr(cmd, '<') != NULL) ||
				 (strstr(cmd, redirect_str1) != NULL) || (strstr(cmd, redirect_str2) != NULL)){

        		char *tok1; // process
            	char *tok2; // file
				if (strchr(cmd, '<') != NULL){
                	tok2 = strtok(cmd, "<");
                	tok1 = tok2;
                	tok2 = strtok(NULL, "<");
            	}else if(strstr(cmd, redirect_str2) != NULL){
					tok2 = strtok(cmd, redirect_str2);
                	tok1 = tok2;
                	tok2 = strtok(NULL, redirect_str2);
				}else if(strstr(cmd, redirect_str1) != NULL){
					tok2 = strtok(cmd, redirect_str1);
                	tok1 = tok2;
                	tok2 = strtok(NULL, redirect_str1);
				}
				else if (strchr(cmd, '>') != NULL){
                	tok2 = strtok(cmd, ">");
                	tok1 = tok2;
                	tok2 = strtok(NULL, ">");
            	}
            	int pid = fork();
            	if (pid == -1) {
        			perror("fork function filed\n"); 
        			exit(EXIT_FAILURE);
    			}
            	if (pid == 0){
					if (tok2[0] == ' '){
						tok2++;
					}
					int file = open(tok2, O_WRONLY | O_CREAT, 0777);
					if (file == -1){
						printf("open file function filed\n");
						return 2;
					}
					dup2(file, STDOUT_FILENO);
					close(file);
						
					char *token;
					token = strtok(tok1, " ");
					char *argv[MAX_CMD_SIZE];
					int i = 0;
					while (token != NULL){
						argv[i] = token;
						token = strtok(NULL, " ");
						i++;
					}
					argv[i] = NULL;

					if (execvp(argv[0], argv) == -1){
						perror("execute function filed\n");  
        				return 1;
					}
				}else{
					waitpid(pid, NULL, 0);
				}
		}
        else{
        	int pid = fork();
        	if (pid == -1){
            	perror("fork function filed\n");  
        		exit(EXIT_FAILURE);
        	}
        	if (pid == 0){
				char *token;
				token = strtok(cmd, " ");
				char *argv[1024];
				int i = 0;
				while (token != NULL){
					argv[i++] = token;
					token = strtok(NULL, " ");
				}
				argv[i] = NULL;
				if (execvp(argv[0], argv) == -1){
					printf("execvp function filed\n");
					return 1;
				}
        	}
        }
    }
    return 0;
}