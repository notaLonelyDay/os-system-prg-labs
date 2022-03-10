

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#define ll long long

ll from_size, to_size, from_date, to_date;
char dir[1024];
int dirLen;

void loadArgInt(char *name, char *from, ll *to) {
	*to = strtol(from, NULL, 10);
	if (errno!=0) {
		fprintf(stderr, "Parameter (%s) must be int.", name);
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
//	printf("%s    %ld  %ld\n", dir, st->st_size, st->st_ctim.tv_sec);
	if (from_size <= st->st_size && st->st_size <= to_size
			&& from_date <= st->st_ctim.tv_sec && st->st_ctim.tv_sec <= to_date)
		printf("%s    %ld  %ld\n", dir, st->st_size, st->st_ctim.tv_sec);
}

void recMain() {
	struct stat st;
	stat(dir, &st);
	if (isDir(&st)) {
		DIR *dirPtr = opendir(dir);

		struct dirent *d;
		while ((d = readdir(dirPtr))) {
			char *destName = d->d_name;
			int destNameLen = strlen(destName);

			if (isValidNextDest(destName)) {
				int oldDirLen = dirLen;

				strcat(dir, "/");
				strcat(dir, destName);
				dirLen += 1 + destNameLen;

				recMain();

				dirLen -= 1 + destNameLen;
				dir[oldDirLen] = '\0';
			}
		}

		closedir(dirPtr);
	} else {
		printFile(&st);
	}

}

int main(int argc, char *argv[]) {
	if (argc < 7) {
		fprintf(stderr, "usage: %s <dir_name> <out_file> <from_size> <to_size> <from_date> <to_date>", argv[0]);
		return 1;
	}

	loadArgInt("from_size", argv[3], &from_size);
	loadArgInt("to_size", argv[4], &to_size);
	loadArgInt("from_date", argv[5], &from_date);
	loadArgInt("to_date", argv[6], &to_date);

	strcpy(dir, argv[1]);
	dirLen = strlen(dir);
	recMain();
	return 0;
}