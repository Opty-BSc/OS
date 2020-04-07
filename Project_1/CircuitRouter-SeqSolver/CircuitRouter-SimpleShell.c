/* 	Exercicio 1 de Sistemas Operativos 

	Grupo 50:
		Sara Machado Nr 86923
		Ricardo Grade Nr 90774
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "../lib/commandlinereader.h"
#include "CircuitRouter-SimpleShell.h"

/* =============================================================================
 * waitChild
 * =============================================================================
 */
/*  waitChild waits for currently active child processes to finish and saves its 
	pid and status on a vector of processes, updating the index value

	@param children: Vector of struct process, containing its info
	@param indVec: Pointer to the index of the previous vector
*/

void waitChild(process *children, int *indVec) {

	int retWait;
	int status;

	retWait = wait(&status);

	if (retWait < 0) {

		if (errno == EINTR)
			waitChild(children, indVec);

		/* ECHILD never actived because of main's nChild variable */

	} else {
		
		children[(*indVec)].pid = retWait;
		children[(*indVec)].status = status;
		(*indVec)++;
	}
}

int main(int argc, char** argv) {

	/* 0 means no limit */
	int MAXCHILDREN = 0;

	int nChild = 0;
	int indVec = 0;
	int maxVec = 10;
	process *children = NULL;

	char buffer[BUFFERSIZE];
	char *argVector[VECTORSIZE];

	int retFork;
	int i;

	/* Verification of the arguments given to CircuitRouter-SimpleShell */
	if (argc > 2) {

		fprintf(stderr, "Too many arguments\n");
		exit(1);

	} else if (argc > 1 && !(MAXCHILDREN = atoi(argv[1]))) {

		fprintf(stderr, "Invalid arguments\n");
		exit(1);
	}

	children = (process *)malloc(sizeof(process) * maxVec);
	assert(children);

	while (1) {

		switch (readLineArguments(argVector, VECTORSIZE, buffer, BUFFERSIZE)) {

			/*
			case 1:

				if (!strcmp(argVector[0], "exit")) {

					Waits for all child processes to finish
					while (indVec < nChild)
						waitChild(children, &indVec);
					
					for (i = 0; i < nChild; i++) {
					
						printf("CHILD EXITED (PID=%d; return %s)\n", children[i].pid,
							 WIFEXITED(children[i].status) && 
							!WEXITSTATUS(children[i].status) ? "OK" : "NOK");
					}

					puts("END.");
					free(children);
					exit(0);
				
				} else {

					fprintf(stderr, "Command not found\n");
					break;
				}
			*/

			case 2:

				if (!strcmp(argVector[0], "run")) {

					if (MAXCHILDREN) {
						/* nChild - indVec = number of active child */
						while ((nChild - indVec) == MAXCHILDREN)
							waitChild(children, &indVec);
					}

					retFork = fork();

					if (retFork < 0) {

						perror("fork");
						free(children);
						exit(1);

					} else if (retFork) {

						/* Verifies if is necessary to increase the size of vector children */
						/* In the end increases nChild */
						if (nChild++ == maxVec) {

							maxVec += 10;
							children = (process *)realloc(children, sizeof(process) * maxVec);
							assert(children);
						}

					} else {

						execl(EXECFILE, EXECFILE, argVector[1], NULL);
						perror("execl");
						free(children);
						exit(1);
					}

				} else if (!strcmp(argVector[0], "exit")) {

					int N = atoi(argVector[1]);

					while (indVec < nChild && N) {
						waitChild(children, &indVec);
						N--;
					}

					if (indVec == nChild) {

						for (i = 0; i < nChild; i++) {
					
							printf("CHILD EXITED (PID=%d; return %s)\n", children[i].pid,
								 WIFEXITED(children[i].status) && 
								!WEXITSTATUS(children[i].status) ? "OK" : "NOK");
						}
						
						puts("END.");
						free(children);
						exit(0);
					}

				} else {

					fprintf(stderr, "Command not found\n");
				}

				break;

			/* When EOF occurs readLineArguments returns -1 */
			case -1:
				fprintf(stderr, "Not able to read line\n");
				free(children);
				exit(1);

			case 0:
				fprintf(stderr, "No arguments\n");
				break;

			default:
				fprintf(stderr, "Invalid arguments\n");
				break;
		}
	}
}
