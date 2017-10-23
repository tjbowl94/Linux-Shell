/*
 *
 *	author:		Thomas Bowlin, 901512915
 *	date:		10/12/17
 *	class:		CSC400
 *
 *	file description:	Simply contains function declarations, so that the main() function in myshell.c
 *	can be placed at the top of the file to be more readable.
 *
 */


char** parse_line();
void execute_command(char** args, char curDir[]);
void display_comment(char** args);
void list_files(char** args, char curDir[]);
void change_directory(char** args, char curDir[]);
void execute(char** args);
void error_msg(char errorMsg[]);
