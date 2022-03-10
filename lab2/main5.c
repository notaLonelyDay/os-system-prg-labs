#include <stdio.h>
#include <dirent.h>


int dirout(char const *path){
	DIR *cur = opendir(path);
	if(!cur){
		fprintf(stderr, "Can't open directory %s!", path);
		return 1;
	}

	struct dirent *d;

	while((d = readdir(cur))){
		printf("%s\n", d->d_name);
	}

	if(closedir(cur)){
		fprintf(stderr,"Can't close directory %s!", path);
		return 1;
	}

	return 0;
}

int main(int argc, char const *argv[])
{
	printf("./:\n");
	dirout("./");
	printf("\n/:\n");
	dirout("/");
	return 0;
}