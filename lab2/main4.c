#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	if (argc < 3){
		printf("Wrong input!\nusage: %s <src_file> <dist_file>", argv[0]);
		return 1;
	}

	FILE* file_1 = fopen(argv[1], "r");

	if (file_1 == NULL){
		printf("File %s can't be opened", argv[1]);
		return 1;
	}

	FILE* file_2 = fopen(argv[2], "w");

	if (file_2 == NULL){
		printf("File %s can't be opened", argv[2]);
		return 1;
	}


	char symb;
	while ((symb = fgetc(file_1)) != EOF){
		fputc(symb, file_2);
	}

	struct stat copy_stat;
	stat(argv[1], &copy_stat);

	chmod(argv[2], copy_stat.st_mode);

	if (fclose(file_1)) {
		printf("File %s can't be written", argv[1]);
		return 1;
	}
	if (fclose(file_2)) {
		printf("File %s can't be written", argv[2]);
		return 1;
	}
}