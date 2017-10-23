/*
 *
 *	author:		Thomas Bowlin, 901512915
 *	date:		10/12/17
 *	class:		CSC400
 *
 *	file description:	This is the main myshell.c file, where most of the logic of the shell program is contained.
 *	It contains the main() function, as well as the function definitions for all functions, excluding system calls
 *	and standard C functions.
 *
 *	The main() function, which contains the main shell loop, is listed first for readability. Therefore, all additional
 *	functions are declared in the myshell.h file.
 *
 */


#include "myshell.h"	// for function declarations
#include "utility.c"	// for other includes, as well as preprocessor macros


/*
 *	main()
 *	main shell loop - displays the prompt (including current directory, stored by the PWD environment variable), then
 *	parses input from the user and executes a command based on the input.
 */
int main()
{
	system("clear");	// clear the console

	char* curDir;	// stores the current file directory
	char** args;	// stores user input

	while (1)	// repeat shell loop until 'quit' command is given by user
	{
		if ((curDir = getenv("PWD")))
			printf(KGRN "%s" KMAG "==>" KNRM, curDir);	// display 'PWD' environment variable

		args = parse_line();	// get user input

		if (strcmp(args[0], "quit") == 0)
		{
			system("clear");
			break;
		}
		else
			execute_command(args, curDir);	// execute command
	}
}


/*
 *	parse_line()
 *	Gets input from the user, then tokenizes the input into individual words. Returns these arguments
 *	in the char** args array
 */
char** parse_line()
{
	char* line = NULL;	// line of user input
	ssize_t bufferSize = 0;
	getline(&line, &bufferSize, stdin);	// get user input

	char** args = malloc(MAX_ARGS * sizeof(char*));	// allocate the args array
	int i;

	args[0] = strtok(line, SEPARATORS);
	i = 1;
	while ((args[i] = strtok(NULL, SEPARATORS)))	// tokenize the input
		i++;

	return args;
}


/*
 *	execute_command()
 *	Reads the values given in args and determines which command to execute based on the given argument. Then
 *	delegates the work by calling specific functions
 */
void execute_command(char** args, char curDir[])
{
	if (strcmp(args[0], "clr") == 0)	// check args[0] (the first word entered by the user)
		system("clear");
	else if (strcmp(args[0], "echo") == 0)
		display_comment(args);	
	else if (strcmp(args[0], "dir") == 0)
		list_files(args, curDir);
	else if (strcmp(args[0], "cd") == 0)
		change_directory(args, curDir);
	else
		execute(args);
}


/*
 *	display_comment()
 *	Displays a comment entered by the user. Achieved with the 'echo' command of the shell. Due to tokenizing the input,
 *	will output all tabs and multiple spaces as a single space.
 */
void display_comment(char** args)
{
	int i = 1;
	while (args[i])	// simply loop through the words entered by user
	{
		printf(KYEL "%s " KNRM, args[i]);	// display in yellow to make it stand out
		i++;
	}
	printf("\n");	
}


/*
 *	list_files()
 *	Lists the files and directories contained within a specific directory. If no additional argument is given, it lists
 *	the contents of the current directory. Otherwise, it lists the contents of the given directory.
 */
void list_files(char** args, char curDir[])
{
	DIR* pDir;	// pointer to the desired directory
	struct dirent* pDirent;	// pointer to directory stream

	if (args[1])	// if the user has specified a directory
	{
		char newDir[MAX_BUFFER];
		strcpy(newDir, curDir);
		strcat(newDir, "/");

		int i = 1;
		while (args[i])	// add the user's input directory to the current directory
		{
			strcat(newDir, args[i]);
			strcat(newDir, " ");
			i++;
		}
		newDir[strlen(newDir) - 1] = 0;

		pDir = opendir(newDir);	// open this directory
	}
	else
	{
		pDir = opendir(curDir);	// if the user did not specify a directory, simply open the current one
	}

	if (pDir == NULL)
	{			
		error_msg("Directory could not be found.");	// error if the directory could not be found
	}
	else
	{
		while ((pDirent = readdir(pDir)) != NULL)
		{
			printf(KBLU "[%s]\t" KNRM, pDirent->d_name);	// print the contents of the directory
		}
	}

	closedir(pDir);	// close directory
	printf("\n");
}


/*
 *	change_directory()
 *	Changes the current working directory, as well as the value of the 'PWD' environment variable. Responds to '..'
 *	by going up one directory, or goes to a specified directory down given by the user.
 */
void change_directory(char** args, char curDir[])
{
	char newDir[MAX_BUFFER];

	if (!args[1])	// the user did not list a directory. Therefore, state the current directory
	{
		printf("Current Directory: %s\n", curDir);
		return;
	}

	if (strcmp(args[1], "..") == 0)	// the user entered 'cd ..'. Therefore, go up a directory
	{
		if (chdir(".."))	// check to make sure directory can be reached
			error_msg("Directory could not be reached.");
		else
		{
			getcwd(newDir, MAX_BUFFER * sizeof(char));
			setenv("PWD", newDir, 1);	// set 'PWD' environment variable
		}
	}
	else	// the user typed a directory, so attempt to navigate to it
	{
		strcpy(newDir, "./");
		int i = 1;
		while (args[i])	// add the given directory to the current directory
		{
			strcat(newDir, args[i]);
			strcat(newDir, " ");
			i++;
		}
		newDir[strlen(newDir) - 1] = 0;

		if (chdir(newDir))	// check to make sure directory exists
			error_msg("Directory does not exist.");
		else
		{
			getcwd(newDir, MAX_BUFFER * sizeof(char));
			setenv("PWD", newDir, 1);	// set 'PWD' environment variable
		}
	}
}


/*
 *	execute()
 *	Runs another program as a new, child process of the program. Using '&' makes the 
 *	new process run in the background
 */
void execute(char** args)
{
	bool bgStatus = false;	// whether or not the new process runs in the bg
	int status;	// status of process
	pid_t child_pid;	// id of the new process

	char* arg = args[0];

	 if (arg[strlen(arg) - 1] == '&')	// check for '&'
	 {
		bgStatus = true;
		arg[strlen(arg) - 1] = 0;
	 }

	child_pid = fork();	// create new process

	 if (child_pid == -1)	// handle errors if process cannot be created
	 {
		error_msg("Could not execute new process.");
		return;
	 }

	 if (child_pid == 0)
		execvp(args[0], args);	// execute new process
	 else
		 if (!bgStatus)	 // if bgStatus isn't true, wait on child process
		 	 waitpid(child_pid, &status, WUNTRACED);
}


/*
 *	error_msg()
 *	Prints an error message to the console, in red text to make it stand out.
 */
void error_msg(char errorMsg[])
{
	printf(KRED "%s\n" KNRM, errorMsg);
}
