#include "../lib/types.h"


/* Maximum size of the request's buffer */
#define BUFF_SIZE 256

/* Maximum number of arguments
   to be read on readLineArguments */
#define N_ARGS 3

/* Default value of MAXCHLD meaning no limit
   of currently active child processes */
#define DEFAULT_MAXCHLD -1

/* Error message for an invalid command */
#define INVALID_COMMAND "Command not supported\n"

/* File to be executed on the <run> command */
#define EXECFILE "../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver"

/* Checks if the maximum capacity has been reached */
#define STEP 10
#define IS_FULL(n) !(n % STEP)

/* Checks for error situations */
#define RET_ERR -1
#define ERR(ret) ret == RET_ERR ? TRUE : FALSE

/* Struct containing the information of a process */
typedef struct {
	pid_t pid;
	int status;
	TIMER_T initial_tim;
	TIMER_T final_tim;
} request;

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
void childHandler(int s);

/* =============================================================================
 * alarmHandler
 * =============================================================================
 	alarmHandler is a Signal handler, which is called when SIGALRM is thrown,
 	which prevents the termination of the process when the signal is thrown.

	@param s: Integer representing the Signal that originated the interruption;
*/
void alarmHandler(int s);

/* =============================================================================
 * sigChildLCK
 * =============================================================================
 	sigChildLCK will BLOCK / UNBLOCK the SIGCHLD.

	@param how: Integer representing the flags BLOCK / UNBLOCK to set the
	signal state;
*/
void sigChildLCK(int how);

/* =============================================================================
 * sigChildWait
 * =============================================================================
 	sigChildWait will wait until the remaining requests its equal to the
 	number of children specified;

	@param nChild: The desired number of currently active child processes;
*/
void sigChildWait(int nChild);

/* =============================================================================
 * execRun
 * =============================================================================
 	execRun will carry out the command <run>.
 	Creates a new child process which will run CircuitRouter-SeqSolver with the
 	specified input file, while the main process will start the clock and
 	saves it, along with the pid, on the struct containing the child's info.

	@param inputFile: The specified input file name, which will be redirected
	to the CircuitRouter-SeqSolver;

	@param fdOut: The output file descripter, which will be given to the
	child process;
*/
void execRun(char *inputFile, int fdOut);

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
void commandExecuter(char *command, int fdOut, const int MAXCHLD, bool_t *recvReq);

/* =============================================================================
 * inpReceiver
 * =============================================================================
 	inpReceiver will receive the command provided.
	Reads the command provided.

	@param fdInp: The input file descripter, where the command comes from;

	@param buffer: Buffer which will contain the command;

	@return: Number of bytes read. In case of error returns -1;
*/
int inpReceiver(int fdInp, char  *buffer);

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
void servReqHandler(const int MAXCHLD, bool_t *recvReq);

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
void cliReqHandler(int fdServ, const int MAXCHLD, bool_t *recvReq);

/* =============================================================================
 * printChildren
 * =============================================================================
 	printChildren will lists all the information of the children created
 	by the main process:
 		- The pid;
 		- The termination state;
 		- The elapsed time;
*/
void printChildren();
