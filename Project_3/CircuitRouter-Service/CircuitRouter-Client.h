#include "../lib/types.h"

/* Maximum size of the request's buffer */
#define BUFF_SIZE 256

/* Checks for error situations */
#define RET_ERR -1
#define ERR(ret) ret == RET_ERR ? TRUE : FALSE

/* =============================================================================
 * recvAnswer
 * =============================================================================
 	recvAnswer will wait for the server's answer.

	@param pipeFileName: The client's pipe name, from where the server's answer
	will reach the client;

	@param buffer: Buffer which will contain the answer;

	@return: Number of bytes read. In case of error returns -1;
*/
int recvAnswer(char *pipeFileName, char *buffer);
