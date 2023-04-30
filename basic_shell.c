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

// int main() {
// 	int i;
// 	char *argv[10];
// 	char command[1024];
// 	char *token;
	
// 	printf("--------------Welcome to stshell--------------\n");
	
// 	while (1) {
// 	    printf("\ncommand: ");
// 	    fgets(command, 1024, stdin);
// 	    command[strlen(command) - 1] = '\0'; // replace \n with \0

// 	    /* parse command line */
// 	    i = 0;
// 	    token = strtok (command," ");
// 	    while (token != NULL)
// 	    {
// 		argv[i] = token;
// 		token = strtok (NULL, " ");
// 		i++;
// 	    }
// 	    argv[i] = NULL;

// 	    if (argv[0] != NULL){

// 		}
// 	}
	
// 	pid_t pid = fork();  // create a new child process
//     if (pid == -1) {
//         perror("fork function filed");  // handle error condition
//         exit(EXIT_FAILURE);
//     }
	
// 	if (pid == 0) {
//         printf("Child process: PID=%d\n", getpid());
        
//     } else {
//         printf("Parent process: PID=%d, child PID=%d\n", getpid(), pid);
//         wait(NULL);
//     }
// 	return 0;
// }


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <netinet/in.h> //structure for storing address information
#include <sys/socket.h> //for socket APIs
#include <sys/types.h>
#include <fcntl.h>

#define TRUE 1
#define MAX_CMD_SIZE 1024

void myPipe(char cmd[MAX_CMD_SIZE]){
    char *tok1;
    char *tok2; 
    tok2 = strtok(cmd, "|");
    tok1 = tok2;
    tok2 = strtok(NULL, "|");

    int fd[2];
    if (pipe(fd) == -1){
        return ;
    }
    int pid1 = fork();
    if (pid1 < 0)
    {
        return ;
    }
    if (pid1 == 0)
    {
        // we stand on the left process child
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);

        char *token;
        token = strtok(tok1, " ");
        char *param[1024];
        int i = 0;
        while (token != NULL)
        {
            param[i++] = token;
            token = strtok(NULL, " ");
        }
        param[i] = NULL;
        printf("%s", param[0]);
        if (execvp(param[0], param) == -1)
        {
            printf("error1");
            return ;
        }
    }

    int pid2 = fork();
    if (pid2 < 0)
    {
        return ;
    }
    if (pid2 == 0)
    {
        // we stand on the right process child
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);

        char *token;
        token = strtok(tok2, " ");
        char *param[1024];
        int i = 0;
        while (token != NULL)
        {
            param[i++] = token;
            token = strtok(NULL, " ");
        }
        param[i] = NULL;
        if (execvp(param[0], param) == -1)
        {
            printf("error");
            return ;
        }
    }
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    close(fd[0]);
    close(fd[1]);
}

int main(){

	printf("--------------Welcome to stshell--------------\n");

    char cmd[MAX_CMD_SIZE] = "";
	const char *ctrl_c_str = "\x03";
	const char *redirect_str1 = ">>";
	const char *redirect_str2 = "<<";

    while (TRUE){
		printf("\ncommand: ");
        fgets(cmd, MAX_CMD_SIZE, stdin);
        cmd[strlen(cmd) - 1] = '\0';

		if(strcmp(cmd, "exit") == 0){
			printf("\n--------------close to stshell--------------\n");
			exit(EXIT_SUCCESS);
		}
		else if(strstr(cmd, ctrl_c_str) != NULL){
			int pid = fork();
			if(pid == -1){
				perror("fork function filed\n");  // handle error condition
        		exit(EXIT_FAILURE);
			}
			if(pid == 0){
				int result = kill(getppid(), SIGSTOP);
				if (result == -1) {
        			perror("kill function filed\n");
        			return 1;
    			}
				printf("to continue the program pleas press 1\n");
				int flag;
				while(TRUE){
					scanf("%d", &flag);
					if (flag == 1){
						int result = kill(getppid(), SIGCONT);
						if (result == -1){
        					perror("kill function filed\n");
        					return 1;
    					}
					}
				}
			}
			else{
				sleep(1);
			}
		}
        else if (strchr(cmd, '|') != NULL){
            myPipe(cmd);
        }
		else if ((strchr(cmd, '>') != NULL) || (strchr(cmd, '<') != NULL)||
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
        			perror("fork function filed\n");  // handle error condition
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

					// test
					printf("%s", argv[0]);

					if (execvp(argv[0], argv) == -1){
						perror("execute function filed\n");  
        				return 1;
					}
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


