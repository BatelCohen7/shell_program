#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

int main(int argc, char *argv[]) {
    int force = 0, verbose = 0, ret = 0;
    FILE *src_file = NULL, *dst_file = NULL;
    char *src_file_path, *dst_file_path;

    // Check for the correct number of arguments
    if (argc < 3) {
        fprintf(stderr, "Usage: copy <file1> <file2> [-v] [-f]\n");
        exit(1);
    }

    // Parse optional flags
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-f") == 0) {
            force = 1;
        }
    }
    int res = copy(verbose, force, argv[2], argv[3]);
    return res;
}