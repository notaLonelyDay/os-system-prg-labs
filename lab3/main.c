//
// Created by user on 4/8/22.
//
#define error(s) { perror(s); exit(1); }
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>

int isDir(const struct stat *st) {
	if (S_ISDIR(st->st_mode)) {
		return 1;
	}
	return 0;
}

int isValidNextDest(const char *dest) {
	if (strcmp(dest, ".")==0 || strcmp(dest, "..")==0) {
		return 0;
	}
	return 1;
}

char src[1024];
int srcLen;
char dest[1024];
int destLen;
int N;
int pCount;

void copyFileRaw(char *srcName, char *destName) {
	FILE *file_1 = fopen(srcName, "r");

	if (file_1==NULL) {
		fprintf(stderr, "File %s can't be opened\n", srcName);
		return;
	}

	FILE *file_2 = fopen(destName, "w");

	if (file_2==NULL) {
		fprintf(stderr, "File %s can't be opened\n", destName);
		return;
	}

	char symb;
	while ((symb = fgetc(file_1))!=EOF) {
		fputc(symb, file_2);
	}

	struct stat copy_stat;
	stat(srcName, &copy_stat);

	chmod(destName, copy_stat.st_mode);

	if (fclose(file_1)) {
		fprintf(stderr, "File %s can't be closed\n", srcName);
		return;
	}
	if (fclose(file_2)) {
		fprintf(stderr, "File %s can't be written\n", destName);
		return;
	}
}

void copyFileNewThread(char *srcName, char *destName) {
	if (pCount >= N) {
		if (wait(NULL)==-1) {
			error("Cant wait for process");
		}
	} else {
		pCount++;
	}
	pid_t child = fork();
	if (child < 0) {
		error("Cant create child process");
	}
	if (child==0) {
		copyFileRaw(srcName, destName);
		exit(0);
	}
}

void recMain() {
	struct stat st;
	if (stat(src, &st)) {
		fprintf(stderr, "File/src (%s) can't be read.\n", src);
		return;
	}
	if (isDir(&st)) {
		mkdir(dest, st.st_mode);
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
				int oldDestLen = destLen;

				strcat(src, "/");
				strcat(src, destName);
				srcLen += 1 + destNameLen;

				strcat(dest, "/");
				strcat(dest, destName);
				destLen += 1 + destNameLen;

				recMain();

				srcLen -= 1 + destNameLen;
				destLen -= 1 + destNameLen;
				src[oldDirLen] = '\0';
				dest[oldDestLen] = '\0';
			}
		}

		if (closedir(dirPtr)) {
			fprintf(stderr, "Can't close directory %s!\n", src);
			return;
		}
	} else {
		copyFileNewThread(src, dest);
	}
}

int main(int argc, char *argv[]) {
	if (argc!=4) {
		fprintf(stderr, "Wrong number of arguments!\n");
		fprintf(stderr, "usage: <src> <dest> <N>\n");
		return 1;
	}

	strcpy(src, argv[1]);
	strcpy(dest, argv[2]);
	N = atoi(argv[3]);

	srcLen = strlen(src);
	destLen = strlen(dest);

	recMain();

	while (wait(NULL)!=-1);

	return 0;
}
