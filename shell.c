#include <stdio.h>	// for fgets
#include <unistd.h>	//I think its for fork
#include <string.h>	// for strtok
#include <stdlib.h> 	// for exit
#include <sys/wait.h>	// for wait
#include <stdbool.h>	// to use boolean data types

bool QUIT = false;

void execute_command (char **command)
{
	int pid;		// pid for the child process
	int status;

	if (strcmp(*command, "quit") == 0)
		QUIT = true;

	if ((pid = fork()) < 0) {
		// Handle forking error
		fprintf(stderr, "Forking failed!!!\n");
		exit(EXIT_FAILURE);

	} else if (pid == 0) {
		// Check for quit command
		if (execvp(*command, command) < 0 ) { // Execution withing child process
			// The if statements runs the command itself
			// so if it returns at all, there's an error
			if (QUIT == true) {
				printf("Quitting program\n");
			} else {
				fprintf(stderr, "Error in exec command!!!\n");
				exit(EXIT_FAILURE);
			}
		}
	} else {
		while((pid = waitpid(-1, &status, 0)) != -1) {
			printf("PID %i exited with status 0\n\n", pid);
		}
	}

	if (QUIT == true)
		exit(EXIT_SUCCESS);
}

void parse_line (char *line, char **commands)
{
	char *token = strtok(line, ";");
	// start a loop to go ver the line separated by ';'
	while (token) {
		// add logic for empty tokens and quit command
		*commands++ = token;
		token = strtok(NULL, ";");
	}
}

void parse_commands (char **commands)
{
	char *single[64];
	int i, j = 0;

	for (i = 0; commands[i] != 0; i++) {
		char *parsable = commands[i];
		char *command_token = strtok(parsable, " \t\n");

		while(command_token) {
			single[j] = command_token;
			command_token = strtok(NULL, " \t\n");
			j++;
		}

		single[j] = (char *)'\0';

		if (*single != 0)
			execute_command(single);
		// Clear the string array for next iteration
		j = 0;
	}
	// clean the commands and single array
	memset(&commands, 0, sizeof(commands));
	memset(&single, 0, sizeof(single));
}

int main(int argc, char *argv[])
{
	char line[512];
	char *commands[64];

	if (argc > 2) {
		fprintf(stderr, "Too many arguments!!!\n");
		exit(EXIT_FAILURE);
	}


	if (argc > 1) {
		FILE *file;
		// Check for the file
		// Open it
		file = fopen(argv[1], "r");
		// Parse it (parsing includes creating the commands array)
		if (file) {
			while (fgets(line, sizeof(line), file)) {
				if (sizeof(line) > 512) {
					fprintf(stderr, "Line too long!!!\n");
					exit(EXIT_FAILURE);
				} else {
					printf(line);
					parse_line(line, commands);
					parse_commands(commands);
				}
			}
		} else {
			fprintf(stderr, "Cannot open file or does not exist!!!\n");
			exit(EXIT_FAILURE);
		}

		// Close the file
		fclose(file);
		// End the program
		printf("Terminaint Program\n");
		exit(EXIT_SUCCESS);
	} else {
		printf("prompt ~> ");
		while (fgets(line, sizeof(line), stdin)) {
			if (sizeof(line) > 512) {
				fprintf(stderr, "Line too long!!!\n");
				exit(EXIT_FAILURE);
			} else {
				parse_line(line, commands);
				parse_commands(commands);
			}
			printf("prompt ~> ");
		}
	}

	return 0;
}
