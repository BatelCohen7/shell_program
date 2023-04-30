#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

int compareFiles(char *file1, char *file2, int verbose, int ignoreCase){
	FILE *f1, *f2;
	char ch1, ch2;
	int result = 0;

	f1 = fopen(file1, "r");
	f2 = fopen(file2, "r");

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

int main(int argc, char *argv[]) {
    int verbose = 0, ignoreCase = 0;

    if (argc < 3) {
        printf("Usage: cmp <file1> <file2> [-v] [-i]\n");
        return 1;
    }

	for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-i") == 0) {
            ignoreCase = 1;
        }
    }

    int result = compareFiles(argv[1], argv[2], verbose, ignoreCase);

    if (result == 0) {
        if (verbose) {
            printf("equal\n");
        }
        return 0;
    }
    else {
        if (verbose) {
            printf("distinct\n");
        }
        return 1;
    }
}