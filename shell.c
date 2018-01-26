#include <stdio.h>	// for fgets
#include <unistd.h>	//I think its for fork
#include <string.h>	// for strtok
#include <stdlib.h> 	// for exit
#include <sys/wait.h>	// for wait
#include <stdbool.h>	// to use boolean data types

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

void execute_command (char **command)
{
	int pid;		// pid for the child process
	int status;
	bool quit = false;

	if ((pid = fork()) < 0) {
		// Handle forking error
		printf("forking failed");
		exit(1);
	} else if (pid == 0) {
		// Check for quit command
		if (strcmp(*command, "ls") == 0)
			sleep(2);

		if (strcmp(*command, "quit") == 0) {
			quit = true;
		} else if (execvp(*command, command) < 0 ) { // Execution withing child process
			// The if statements runs the command itself
			// so if it returns at all, there's an error
			printf("there was an error with the exec command\n");
			exit(1);
		}
	} else {
		while(wait(&status) != pid)
			// Wait for child exec process to do its thing
			;
		printf("Child process finished with id: %d\n\n\n", pid);
	}
}

int main(void)
{
	char line[1024];
	char *commands[64];
	char *single[64];
	int i, j = 0;

	printf("prompt ~> ");
	while (fgets(line, sizeof(line), stdin)) {
		parse_line(line, commands);
		
		// Iterate over every command
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
		// execute_commands(command);
		printf("prompt ~> ");
	}
	return 0;
}
