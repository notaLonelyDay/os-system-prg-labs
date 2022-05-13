#include <stdio.h>
#include <semaphore.h>
#include <malloc.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#define CHILD_COUNT 8

typedef struct Node {
	int val;
	struct Node *cn[CHILD_COUNT];
	int signal; // -1 - no signal
	int aliveChildCount;
	sigset_t *sigset;
} Node;

Node *curNode = NULL;

void signalHandler(int sig, siginfo_t *siginfo, void *code) {
	printf("Signal %d\n", sig);
}

Node *createNode(int val, Node *parent, int signal) {
	Node *n = (Node *)malloc(sizeof(Node));
	n->signal = signal;
	n->val = val;
	n->aliveChildCount = 0;
	n->sigset = (sigset_t *)malloc(sizeof(sigset_t));
	for (int i = 0; i < CHILD_COUNT; ++i) {
		n->cn[i] = NULL;
	}
	if (parent!=NULL) {
		char nodeAdded = 0;
		for (int i = 0; i < CHILD_COUNT; ++i) {
			if (parent->cn[i]==NULL) {
				parent->cn[i] = n;
				nodeAdded = 1;
				break;
			}
		}
		if (!nodeAdded) {
			perror("Error: node not added\n");
		}
	}
	return n;
}

void createTree(Node *root) {
	Node *n1 = createNode(1, root, -1);
	Node *n2 = createNode(2, n1, SIGUSR2);
	Node *n3 = createNode(3, n1, SIGUSR2);
	Node *n4 = createNode(4, n1, SIGUSR2);
	createNode(5, n1, SIGUSR2);

	createNode(6, n2, SIGUSR1);

	createNode(7, n3, SIGUSR1);
	createNode(8, n4, SIGUSR1);
}

void registerSignalHandler(Node *n) {
	if (n->signal==-1) return;

	if (sigemptyset(n->sigset) ||
			sigaddset(n->sigset, n->signal)) {
		perror("Error: adding signal handler to set");
	}
	if (sigprocmask(SIG_UNBLOCK, n->sigset, NULL) < 0) {
		perror("Error: setting signal mask");
	}

	struct sigaction act;
	act.sa_sigaction = signalHandler;
	act.sa_mask = *(n->sigset);
	act.sa_flags = SA_SIGINFO;

	if (sigaction(n->signal, &act, 0)) {
		perror("Error: registering signal handler");
	}
}


pid_t forkProcess(Node *n) {
	pid_t pid = fork();
	switch (pid) {
		case -1: perror("Error: fork failed\n");
			break;
		case 0:
			// for child process
			printf("Forked process %d(%d) from %d(%d)\n", getpid(), n->val, getppid(), curNode->val);
			curNode = n;
			registerSignalHandler(curNode);

			return 0;
		default:
			// for parent process
			n->aliveChildCount++;
			return pid;
	}
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
void createProcessTree(Node *root) {
	curNode = root;
	for (int i = 0; i < CHILD_COUNT; ++i) {
		struct Node *nextNode = curNode->cn[i];
		if (nextNode!=NULL) {
			char semName[255] = "/child_register_handler";
			size_t strlen1 = strlen(semName);
			semName[strlen1] = '0' + nextNode->val;
			semName[strlen1 + 1] = '\0';
//			printf("semName: %s\n", semName);
			sem_t *childRegisterHandler = sem_open(semName, O_CREAT, 0777, 0);
//			sem_init(childRegisterHandler, 1, 0);
			pid_t pid = forkProcess(nextNode);
//			printf("Creating process tree for %d\n", curNode->val);
			if (pid==0) {
				// for child process
//				printf("Creating process tree for %d\n", curNode->val);
				createProcessTree(curNode);
				sem_post(childRegisterHandler);
//				printf("Exiting %d\n", curNode->val);
				exit(0);
			} else {
				// for parent process
				sem_wait(childRegisterHandler);
				printf("Process tree for %d created\n", nextNode->val);
				sem_destroy(childRegisterHandler);
				if (nextNode->signal!=-1 && curNode->val==1) {
//					printf("NexNode %d\n", nextNode->val);
//					printf("CurNode %d\n", curNode->val);
					kill(pid, SIGUSR2);
				}
				continue;
			}
		}
	}

	// TODO: wait for all child processes chessk
//	while (1){};
	while (wait(NULL)!=-1);
//	printf("Finished %d\n", curNode->val);
}
#pragma clang diagnostic pop

int main() {
	Node *root = createNode(0, NULL, -1);
	createTree(root);
	createProcessTree(root);

	return 0;
}