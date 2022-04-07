#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#define ll long long

ll from_size, to_size, from_date, to_date;
char src[1024];
int srcLen;
FILE *outFile;

void loadArgInt(char *name, char *from, ll *to) {
	*to = strtol(from, NULL, 10);
	if (errno!=0) {
		fprintf(stderr, "Parameter (%s) must be int.\n", name);
		exit(1);
	}
}

int isValidNextDest(const char *dest) {
	if (strcmp(dest, ".")==0 || strcmp(dest, "..")==0) {
		return 0;
	}
	return 1;
}

int isDir(const struct stat *st) {
	if (S_ISDIR(st->st_mode)) {
		return 1;
	}
	return 0;
}

void printFile(const struct stat *st) {
//	printf("%s    %ld  %ld\n", src, st->st_size, st->st_ctim.tv_sec);
	if (from_size <= st->st_size && st->st_size <= to_size
			&& from_date <= st->st_ctim.tv_sec && st->st_ctim.tv_sec <= to_date)
		printf("%s    %ld  %ld\n", src, st->st_size, st->st_ctim.tv_sec);
	fprintf(outFile, "%s    %ld  %ld\n", src, st->st_size, st->st_ctim.tv_sec);
}

void recMain() {
	struct stat st;
	if (stat(src, &st)) {
		fprintf(stderr, "File/src (%s) can't be read.\n", src);
		return;
	}
	if (isDir(&st)) {
		DIR *dirPtr = opendir(src);
		if (!dirPtr) {
			fprintf(stderr, "Can't open directory %s!\n", src);
			return;
		}

		struct dirent *d;
		while ((d = readdir(dirPtr))) {
			char *destName = d->d_name;
			int destNameLen = strlen(destName);

			if (isValidNextDest(destName)) {
				int oldDirLen = srcLen;

				strcat(src, "/");
				strcat(src, destName);
				srcLen += 1 + destNameLen;

				recMain();

				srcLen -= 1 + destNameLen;
				src[oldDirLen] = '\0';
			}
		}

		if (closedir(dirPtr)) {
			fprintf(stderr, "Can't close directory %s!\n", src);
			return;
		}
	} else {
		printFile(&st);
	}

}

int main(int argc, char *argv[]) {
	if (argc < 7) {
		fprintf(stderr, "usage: %s <dir_name> <out_file> <from_size> <to_size> <from_date> <to_date>", argv[0]);
		return 1;
	}

	outFile = fopen(argv[2], "w");
	if (outFile==NULL) {
		fprintf(stderr, "File %s can't be created", argv[2]);
		return 1;
	}

	loadArgInt("from_size", argv[3], &from_size);
	loadArgInt("to_size", argv[4], &to_size);
	loadArgInt("from_date", argv[5], &from_date);
	loadArgInt("to_date", argv[6], &to_date);

	strcpy(src, argv[1]);
	srcLen = strlen(src);
	recMain();

	if (fclose(outFile)) {
		fprintf(stderr, "File %s can't be closed", argv[2]);
		return 1;
	}
	return 0;
}