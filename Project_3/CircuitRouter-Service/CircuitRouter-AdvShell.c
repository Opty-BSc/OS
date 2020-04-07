/* 	Exercicio 3 de Sistemas Operativos

	Servidor

	Grupo 50:
		Sara Machado Nr 86923
		Ricardo Grade Nr 90774
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../lib/types.h"
#include "../lib/timer.h"
#include "../lib/commandlinereader.h"
#include "../lib/stringmanipulator.h"
#include "CircuitRouter-AdvShell.h"

/* Global variables */
/* Number of requests */
int nReqs;
/* Number of remaining requests */
int remReqs;
/* List of requests */
request *reqList;

/* =============================================================================
 * childHandler
 * =============================================================================
 	childHandler is a Signal handler which is called when SIGCHLD is thrown,
 	which means, that a child process has finished.
 	When the Signal is thrown, the function will:
 		- Stop the clock;
 		- Save the status and the elapsed time on the struct containing the 
 		terminated child's info;

	@param s: Integer representing the Signal that originated the interruption
*/
void childHandler(int s) {

	int i;
	pid_t pid;
	int status;
	TIMER_T final_tim;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {

		/* Stopping the clock */
		TIMER_READ(final_tim);
		remReqs--;
		for (i = 0; i < nReqs; i++) {
			if (reqList[i].pid == pid) {
				reqList[i].final_tim = final_tim;
				reqList[i].status = status;
				break;
			}
		}
	}
}

/* =============================================================================
 * alarmHandler
 * =============================================================================
 	alarmHandler is a Signal handler, which is called when SIGALRM is thrown,
 	which prevents the termination of the process when the signal is thrown.

	@param s: Integer representing the Signal that originated the interruption;
*/
void alarmHandler(int s) {}

/* =============================================================================
 * sigChildLCK
 * =============================================================================
 	sigChildLCK will BLOCK / UNBLOCK the SIGCHLD.

	@param how: Integer representing the flags BLOCK / UNBLOCK to set the
	signal state;
*/
void sigChildLCK(int how) {

	static sigset_t sigSetLCK;
	static bool_t firstCall = TRUE;

	if (firstCall) {
		firstCall = FALSE;
		/* Initializing mask with SIGCHLD */
		sigemptyset(&sigSetLCK);
		sigaddset(&sigSetLCK, SIGCHLD);
	}

	if (ERR(sigprocmask(how, &sigSetLCK, NULL))) {
		perror("sigprocmask");
		exit(EXIT_FAILURE);
	}
}

/* =============================================================================
 * sigChildWait
 * =============================================================================
 	sigChildWait will wait until the remaining requests its equal to the
 	number of children specified;

	@param nChild: The desired number of currently active child processes;
*/
void sigChildWait(int nChild) {

	sigChildLCK(SIG_BLOCK);

	while (remReqs > nChild) {

		sigChildLCK(SIG_UNBLOCK);

		ualarm(100000, 100000);
		pause();
		ualarm(0, 0);

		sigChildLCK(SIG_BLOCK);
	}

	sigChildLCK(SIG_UNBLOCK);
}

/* =============================================================================
 * execRun
 * =============================================================================
 	execRun will carry out the command <run>.
 	Creates a new child process which will run CircuitRouter-SeqSolver with the
 	specified input file, while the main process will start the clock and
 	save it, along with the pid, on the struct containing the child's info.

	@param inputFile: The specified input file name, which will be redirected
	to the CircuitRouter-SeqSolver;

	@param fdOut: The output file descripter, which will be given to the
	child process;
*/
void execRun(char *inputFile, int fdOut) {

	pid_t retFork;
	int index;
	TIMER_T initial_tim;

	sigChildLCK(SIG_BLOCK);

	/* Starting the clock */
	TIMER_READ(initial_tim);
	if (ERR((retFork = fork()))) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	/* Main process */
	if (retFork) {

		index = nReqs++;
		remReqs++;

		/* Increases the size of the request list if necessary */
		if (IS_FULL(nReqs)) {
			reqList = (request *)realloc(reqList, sizeof(request) * (nReqs + STEP));
			assert(reqList);
		}

		/* Saving the process info */
		reqList[index].pid = retFork;
		reqList[index].initial_tim = initial_tim;

	/* Child process */
	} else {

		if (fdOut != 1) {
			/* Redirects the child process output
			   to the corresponding client's pipe */
			close(1);
			dup(fdOut);
			close(fdOut);
		}

		/* Runs CircuitRouter-SeqSolver */
		execl(EXECFILE, EXECFILE, inputFile, NULL);
		perror("execl");
		exit(EXIT_FAILURE);
	}

	sigChildLCK(SIG_UNBLOCK);
}

/* =============================================================================
 * commandExecuter
 * =============================================================================
 	commandExecuter will carry out the command provided.
 	Reads the arguments of the command, and associates it to the desired action.

	@param command: The command provided to be executed;

	@param fdOut: The output file descripter, which will be given to the
	child process;

	@param MAXCHLD: The constant associated to the maximum number of processes
	that can be created by main process;

	@param recvReq: The reference of the boolean that indicates if the server
	is still receiving requests;
*/
void commandExecuter(char *command, int fdOut, const int MAXCHLD, bool_t *recvReq) {

	char *argVector[N_ARGS + 1];
	int nTokens;

	if (ERR((nTokens = readLineArguments(argVector, N_ARGS + 1, command, BUFF_SIZE)))) {
		perror("readLineArguments");
		exit(EXIT_FAILURE);
	}

	/* recvReq is NULL if the executer doesn't have permissions
	   to change the state of the server */
	if (nTokens == 1 && recvReq != NULL && !strcmp(argVector[0], "exit")) {

		/* Stops receiving requests */
		*recvReq = FALSE;
		return;
	}

	if (nTokens == 2 && !strcmp(argVector[0], "run")) {

		/* Checks if MAXCHLD was set */
		if (MAXCHLD != DEFAULT_MAXCHLD) {
			sigChildWait(MAXCHLD-1);
		}

		execRun(argVector[1], fdOut);
		return;
	}

	if (ERR(write(fdOut, INVALID_COMMAND, sizeof(INVALID_COMMAND)))) {
		perror("write");
		exit(EXIT_FAILURE);
	}
}

/* =============================================================================
 * inpReceiver
 * =============================================================================
 	inpReceiver will receive the command provided.
	Reads the command provided.

	@param fdInp: The input file descripter, where the command comes from;

	@param buffer: Buffer which will contain the command;

	@return: Number of bytes read. In case of error returns -1;
*/
int inpReceiver(int fdInp, char  *buffer) {

	int nBytes;

	if (ERR((nBytes = read(fdInp, buffer, BUFF_SIZE)))) {
		perror("read");
		exit(EXIT_FAILURE);
	}

	if (!nBytes) {
		fprintf(stderr, "EOF\n");
		return -1;
	}

	if (nBytes == BUFF_SIZE) {
		fprintf(stderr, "Buffer limit exceeded\n");
		return -1;
	}

	buffer[nBytes] = '\0';
	return nBytes;
}

/* =============================================================================
 * servReqHandler
 * =============================================================================
 	servReqHandler will carry out the command provided by the server.
	Reads the command, checks it, and finally executes it.

	@param MAXCHLD: The constant associated to the maximum number of processes
	that can be created by main process;

	@param recvReq: The reference of the boolean that indicates if the server
	is still receiving requests;
*/
void servReqHandler(const int MAXCHLD, bool_t *recvReq) {

	char buffer[BUFF_SIZE];
	char *command;
	char *savePtr;

	if (ERR(inpReceiver(0, buffer))) {
		*recvReq = FALSE;
		return;
	}

	command = strtok_r(buffer, "\n", &savePtr);

	/* While there's a command to execute */
	while (command != NULL) {

		commandExecuter(command, 1, MAXCHLD, recvReq);

		command = strtok_r(NULL, "\n", &savePtr);
	}
}

/* =============================================================================
 * cliReqHandler
 * =============================================================================
 	cliReqHandler will carry out the command provided by the client.
	Reads the command, checks it, and finally executes it.

	@param fdServ: The client's pipe descripter, that sends the request;

	@param MAXCHLD: The constant associated to the maximum number of processes
	that can be created by main process;

	@param recvReq: The reference of the boolean that indicates if the server
	is still receiving requests;
*/
void cliReqHandler(int fdServ, const int MAXCHLD, bool_t *recvReq) {

	char buffer[BUFF_SIZE];
	int fdCli;
	char *command;
	char *cliPipeName;
	char *savePtr;

	if (ERR(inpReceiver(fdServ, buffer))) {
		*recvReq = FALSE;
		return;
	}

	command = strtok_r(buffer, "\n", &savePtr);

	/* While there's a command to execute */
	while (command != NULL) {

		cliPipeName = strtok_r(NULL, "\n", &savePtr);
		assert(cliPipeName);

		if (ERR((fdCli = open(cliPipeName, O_WRONLY)))) {
			perror("open");
			exit(EXIT_FAILURE);
		}

		commandExecuter(command, fdCli, MAXCHLD, NULL);
		close(fdCli);

		command = strtok_r(NULL, "\n", &savePtr);
	}
}

/* =============================================================================
 * printChildren
 * =============================================================================
 	printChildren will lists all the information of the children created
 	by the main process:
 		- The pid;
 		- The termination state;
 		- The elapsed time;
*/
void printChildren() {

	for (int i = 0; i < nReqs; i++) {
					
		printf("CHILD EXITED (PID=%d; return %s; %d s)\n", 
			/* Pid */
			reqList[i].pid, 
			/* Termination state */
			WIFEXITED(reqList[i].status) && !WEXITSTATUS(reqList[i].status)
			? "OK" : "NOK", 
			/* Elapsed time */
			(int)TIMER_DIFF_SECONDS(reqList[i].initial_tim, reqList[i].final_tim));
	}

	puts("END.");
}


int main(int argc, char **argv) {

	if (argc > 2) {
		fprintf(stderr, "Invalid number of arguments\n");
		exit(EXIT_FAILURE);
	}

	int MAXCHLD = DEFAULT_MAXCHLD;
	if (argc == 2 && (MAXCHLD = atoi(argv[1])) < 1) {
		fprintf(stderr, "Invalid argument\n");
		exit(EXIT_FAILURE);
	}

	/* Initializing Global variables */
	nReqs = remReqs = 0;
	reqList = (request *)malloc(sizeof(request) * STEP);
	assert(reqList);

	/* Initializing sigaction's act for SIGCHLD */
	struct sigaction sigChildTreat;
	sigChildTreat.sa_handler = childHandler;
	sigemptyset(&sigChildTreat.sa_mask);
	sigChildTreat.sa_flags = SA_NOCLDSTOP | SA_RESTART;

	if (ERR(sigaction(SIGCHLD, &sigChildTreat, NULL))) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	/* Initializing sigaction's act for SIGALRM */
	struct sigaction sigAlarmTreat;
	sigAlarmTreat.sa_handler = alarmHandler;
	sigemptyset(&sigAlarmTreat.sa_mask);

	if (ERR(sigaction(SIGALRM, &sigChildTreat, NULL))) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	char *pipeFileName = createPipeName("", argv[0]);
	assert(pipeFileName);

	if (ERR(remove(pipeFileName))) {
		/* Checks if there was an error different 
		   than a non existent file error */
		if (errno != ENOENT) {
			perror("remove");
			exit(EXIT_FAILURE);
		}
	}

	/* Creating the server's pipe */
	if (ERR(mkfifo(pipeFileName, 0777))) {
		perror("mkfifo");
		exit(EXIT_FAILURE);
	}

	int fdServ;
	/* Opening the server's pipe to RD / WR
	   so it becomes non blocking */
	if (ERR((fdServ = open(pipeFileName, O_RDWR)))) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	fd_set mask;
	bool_t recvReq;

	/* Starts receiving requests */
	recvReq = TRUE;
	while (recvReq) {

		/* Inicializing select's mask */
		FD_ZERO(&mask);
		FD_SET(0, &mask);
		FD_SET(fdServ, &mask);

		while (ERR(select(fdServ + 1, &mask, NULL, NULL, NULL))) {

			if (errno == EINTR)
				continue;

			perror("select");
			exit(EXIT_FAILURE);
		}

		if (FD_ISSET(0, &mask)) {

			servReqHandler(MAXCHLD, &recvReq);
		}

		if (FD_ISSET(fdServ, &mask)) {
			
			cliReqHandler(fdServ, MAXCHLD, &recvReq);
		}
	}


	sigChildWait(0);
	printChildren();

	free(reqList);
	close(fdServ);
	remove(pipeFileName);
	free(pipeFileName);
	exit(EXIT_SUCCESS);
}
