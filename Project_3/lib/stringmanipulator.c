/* 	Exercicio 3 de Sistemas Operativos

	Grupo 50:
		Sara Machado Nr 86923
		Ricardo Grade Nr 90774
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stringmanipulator.h"

/* =============================================================================
 * createPathName
 * =============================================================================
 	createPathName will receive:
 		- The directory's name;
 		- The file's name;
 		- The extension's name;
 	Creates the path name with the given arguments.

	@param prefix: Char* referring to the directory's name;

	@param name: Char* referring to the file's name;

	@param sufix: Char* referring to the extension's name;

	@return: Path name.
*/
char *createPathName(char *prefix, char *name, char *sufix) {

	size_t pathLen = strlen(prefix) + strlen(name) + strlen(sufix);
	char *pathName = (char *)malloc(sizeof(char) * (pathLen + 1));
	assert(pathName);

	strcpy(pathName, prefix);
	strcat(pathName, name);
	strcat(pathName, sufix);

	return pathName;
}

/* =============================================================================
 * createPipeName
 * =============================================================================
 	createPipeName will receive:
 		- The directory's name;
 		- The file's name;
 	Creates the path name with the given arguments and the extension .pipe.

	@param prefix: Char* referring to the directory's name;

	@param name: Char* referring to the file's name;

	@return: Pipe's path name.
*/
char *createPipeName(char *prefix, char *name) {

	return createPathName(prefix, name, ".pipe");
}

/* =============================================================================
 * createPipeName
 * =============================================================================
 	createPipeName will receive:
 		- The directory's name;
 	Creates a new directory, with a random name, inside the specified
 	directory (prefix).

	@param prefix: Char* referring to the directory's name;

	@return: New directory's path name.
*/
char *createDir(char *prefix) {

	char *pathName = createPathName(prefix, "XXXXXX", "");

	if (mkdtemp(pathName) == NULL) {
		perror("mkdtemp");
		return NULL;
	}

	pathName = (char *)realloc(pathName, sizeof(char) * (strlen(pathName) + 2));
	strcat(pathName, "/");

	return pathName;
}
