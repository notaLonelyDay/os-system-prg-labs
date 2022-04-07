//
// Created by user on 4/8/22.
//
#define error(s) { perror(s); exit(1); }
#include <time.h>
#include <wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void print_info(char *prefix) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	time_t timer;
	char buffer[26];
	struct tm *tm_info;

	timer = time(NULL);
	tm_info = localtime(&timer);

	strftime(buffer, 26, "%H:%M:%S", tm_info);

	printf("[%s]:    pid: %d    ppid: %d     time: %s:%d\n\n",
	       prefix, getpid(), getppid(), buffer, (int)ts.tv_nsec/1000000);
}

int main(int argc, char *argv[]) {

	print_info("parent");

	pid_t pid;
	for (int i = 0; i < 2; i++) {
		pid = fork();

		if (pid==0) {

			char buff[] = "child 0";
			buff[6] += i;
			print_info(buff);

			exit(0);
		} else if (pid < 0) {
			error("ERROR CREATING CHILD PROCESS");
		}
	}

	if (pid > 0) {
		system("ps -x");
		if (wait(NULL)==-1) {
			perror("Wait failure");
		}
		if (wait(NULL)==-1) {
			perror("Wait failure");
		}
	}

}