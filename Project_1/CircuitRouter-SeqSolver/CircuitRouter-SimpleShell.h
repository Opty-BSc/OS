
/* N-1 strings allowed in a input line */
/* The value is bigger than necessary to capture the "Too many arguments" error */
#define VECTORSIZE 4
/* N-1 caracters allowed in a input line */
#define BUFFERSIZE 256
/* Name of the executable file */
#define EXECFILE "./CircuitRouter-SeqSolver"

/* Struct used to save Child's process info */
typedef struct {

	int pid;
	int status;
} process;

void waitChild(process *children, int *indVec);