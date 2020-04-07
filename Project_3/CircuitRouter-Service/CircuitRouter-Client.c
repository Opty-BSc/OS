/* 	Exercicio 3 de Sistemas Operativos

	Cliente

	Grupo 50:
		Sara Machado Nr 86923
		Ricardo Grade Nr 90774
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../lib/stringmanipulator.h"
#include "CircuitRouter-Client.h"

/* =============================================================================
 * recvAnswer
 * =============================================================================
 	recvAnswer will wait for the server's answer.

	@param pipeFileName: The client's pipe name, from where the server's answer
	will reach the client;

	@param buffer: Buffer which will contain the answer;

	@return: Number of bytes read. In case of error returns -1;
*/
int recvAnswer(char *pipeFileName, char *buffer) {

	int fdCli;
	int nBytes;

	/* Opening the client's pipe to RD */
	if (ERR((fdCli = open(pipeFileName, O_RDONLY)))) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	/* Reading the server's answer */
	if (ERR((nBytes = read(fdCli, buffer, BUFF_SIZE)))) {
		perror("read");
		exit(EXIT_FAILURE);
	}

	close(fdCli);

	return !nBytes ? -1 : nBytes;
}


int main(int argc, char** argv) {

	if (argc != 2) {
		fprintf(stderr, "Invalid number of arguments\n");
		exit(EXIT_FAILURE);
	}

	int fdServ;
	char *pipePathName;
	char *pipeFileName;
	size_t pipeLen;
	char buffer[BUFF_SIZE];
	int nBytes;

	pipePathName = createDir("/tmp/");
	assert(pipePathName);
	pipeFileName = createPipeName(pipePathName, "Client");
	assert(pipeFileName);
	pipeLen = strlen(pipeFileName);

	/* Creating the client's pipe */
	if (ERR(mkfifo(pipeFileName, 0777))) {
		perror("mkfifo");
		exit(EXIT_FAILURE);
	}

	/* Opening the server's pipe to WR */
	if (ERR((fdServ = open(argv[1], O_WRONLY)))) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	while (1) {

		/* Receiving requests from stdin */
		if (fgets(buffer, BUFF_SIZE, stdin) == NULL) {
			
			/* In case of EOF */
			if (feof(stdin)) {
				fprintf(stderr, "EOF\n");
				break;
			}

			perror("fgets");
			exit(EXIT_FAILURE);
		}

		if (!strcmp(buffer, "exit\n")) {
			break;
		}

		/* Ignoring empty commands */
		if (!strcmp(buffer, "\n")) {
			continue;
		}

		/* Warning for over the limit commands */
		if (strlen(buffer) + pipeLen + 1 > BUFF_SIZE - 1) {
			fprintf(stderr, "Buffer limit exceeded\n");
			/* Cleaning stdin */
			while (getchar() != '\n');
			continue;
		}

		/* Including client's pipe name on request */
		strcat(buffer, pipeFileName);
		strcat(buffer, "\n");

		/* Sending a request to the server's pipe */
		if (ERR(write(fdServ, buffer, strlen(buffer)))) {
			perror("write");
			exit(EXIT_FAILURE);
		}

		/* Waiting for the server's answer */
		if (ERR((nBytes = recvAnswer(pipeFileName, buffer)))) {
			fprintf(stderr, "Error processing request\n");
			continue;
		}

		if (ERR(write(1, buffer, nBytes))) {
			perror("write");
			exit(EXIT_FAILURE);
		}
	}

	close(fdServ);
	remove(pipeFileName);
	remove(pipePathName);
	free(pipeFileName);
	free(pipePathName);
	exit(EXIT_SUCCESS);
}
