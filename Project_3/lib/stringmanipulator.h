
#ifndef STRINGMANIPULATOR_H
#define STRINGMANIPULATOR_H

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
char *createPathName(char *prefix, char *name, char *sufix);

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
char *createPipeName(char *prefix, char *name);

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
char *createDir(char *prefix);

#endif
