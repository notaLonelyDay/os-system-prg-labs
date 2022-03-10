#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {

	if (argc < 3) {
		printf("Wrong args\nusage: %s <file_name> <amount_of_output_strings>", argv[0]);
		return 1;
	}

	char *file_name = argv[1];

	char *n_str = argv[2];
	int n = strtol(n_str, NULL, 10);
	if (errno!=0) {
		printf("Last parameter (%s) must be int.", n_str);
		return 1;
	}

	FILE *file = fopen(file_name, "r");

	if (file==NULL) {
		printf("Error: can't open file %s", argv[1]);
		return 1;
	}

	char symb;
	int printed = 0;
	while ((symb = fgetc(file))!=EOF) {
		if (symb=='\n')
			printed++;
		if (printed==n && n!=0) {
			getc(stdin);
			printed = 0;
		}

		putc(symb, stdout);
	}
	if (fclose(file)) {
		printf("File %s can't be written", file_name);
		return 1;
	}
	return 0;
}