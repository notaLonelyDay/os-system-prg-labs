#include <stdio.h>

int main(int argc, char *argv[]) {
	if (argc < 2){
		printf("bad args\n usage: %s <file_name>", argv[0]);
		return 1;
	}

	char* file_name = argv[1];

	FILE* file = fopen(file_name, "w");

	if (file == NULL){
		printf("File %s can't be created", file_name);
		return 1;
	}

	char symb;
	while ((symb = getc(stdin)) != 6){
		fputc(symb, file);
	}

	if(fclose(file)){
		printf("File %s can't be written", file_name);
		return 1;
	}
	return 0;
}
