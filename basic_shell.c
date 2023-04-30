#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include <string.h>
#include <ctype.h>

// Part A:
int cmp(char *file1, char *file2, int verbose, int ignoreCase)
{
	FILE *f1, *f2;
	char ch1, ch2;
	int result = 0;

	f1 = fopen(file1, "r");
	f2 = fopen(file2, "r");
	// i need to chack if use "r" or "rb" if the compare between two file text or all the types

	if (f1 == NULL || f2 == NULL)
	{
		fprintf(stderr, "Error opening one of the files: %s\n", strerror(errno));
		goto cleanup;
	}

	while (1)
	{
		ch1 = fgetc(f1);
		ch2 = fgetc(f2);

		// we chack if finish to read the file
		if (ch1 == EOF || ch2 == EOF)
		{
			break;
		}

		if (ignoreCase)
		{
			ch1 = tolower(ch1);
			ch2 = tolower(ch2);
		}

		if (ch1 != ch2)
		{
			result = 1;
			if(verbose){printf("thw files are distinct");}
			return result;
		}
	}
	if(verbose)
	{
		printf("thw files are equal");
	}


cleanup:
	if (f1 != NULL)
	{
		fclose(f1);
	}
	if (f2 != NULL)
	{
		fclose(f2);
	}
	return result;
}

int copy(int verbos, int force, char *src_file_path, char *dest_file_path){
	FILE *src_file = NULL;
	FILE *dest_file = NULL;
	int result = 0;
	src_file = fopen(src_file_path,"rb");
	if(src_file == NULL){
		fprintf(stderr,"Error opening source file:%s\n",strerror(errno));
		result = 1;
		goto cleanup;
	}
	dest_file = fopen(dest_file_path, force?"wb":"rb");
	if(dest_file == NULL){
		if(errno == EEXIST && !(force)){
			fprintf(stderr,"Target file already exist\n");
			result = 1;
			goto cleanup;
		}else{
			fprintf(stderr,"Error opening file:%s",strerror(errno));
			result = 1;
			goto cleanup;
		}
	}
	int c;
	while(((c = fgetc(src_file)) != EOF)){
		fputc(c, dest_file);
	}
	if (verbos){
		printf("Success\n");
	}

	cleanup:
	if (src_file != NULL)
	{
		fclose(src_file);
	}
	if (dest_file != NULL)
	{
		fclose(dest_file);
	}
	return result;
}

int main() {
	int i;
	char *argv[10];
	char command[1024];
	char *token;

	while (1) {
	    printf("hello: ");
	    fgets(command, 1024, stdin);
	    command[strlen(command) - 1] = '\0'; // replace \n with \0

	    /* parse command line */
	    i = 0;
	    token = strtok (command," ");
	    while (token != NULL)
	    {
		argv[i] = token;
		token = strtok (NULL, " ");
		i++;
	    }
	    argv[i] = NULL;

	    /* Is command empty */
	    if (argv[0] == NULL)
		continue;

	    /* for commands not part of the shell command language */
	    if (fork() == 0) {
		execvp(argv[0], argv);
		wait(NULL);
	    }
	}
}
