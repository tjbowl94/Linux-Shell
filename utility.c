/*
 *
 *	author: 	Thomas Bowlin, 901512915
 *	date: 		10/12/17
 *	class:		CSC400
 *
 *	file description:	Utility.c contains includes used by the program, as well as definitions
 *	for preprocessor macros used by the program. This enhances code readability in the main myshell.c 
 *	file.
 *
 */


#include <dirent.h>	// for directory streams
#include <errno.h>	// for linux error reporting
#include <string.h>	// for C strings
#include <stdbool.h>	// for boolean type
#include <stdio.h>	// for C I/O operations
#include <stdlib.h>	// for C standard library
#include <unistd.h>	// for POSIX O.S. API
#include <sys/types.h>	// for definitions of special types
#include <sys/wait.h>	// for constants used with waitpid()


#define MAX_BUFFER 1024	// used as generic buffer size when declaring character arrays
#define MAX_ARGS 64	// used as maximum number of arguments when getting user input
#define SEPARATORS " \t\n"	// used to tokenize user input into words


#define KNRM "\x1B[0m"	// used to format shell text output to make it more readable
#define KRED "\x1B[31m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KGRN "\x1B[32m"
#define KMAG "\x1B[35m"
