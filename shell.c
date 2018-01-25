#include <stdio.h>	// for fgets
#include <unistd.h>	//I think its for fork
#include <string.h>	// for strtok
#include <stdlib.h> 	// for exit
#include <sys/wait.h>	// for wait

void parse_interactive (char *line, char **command)
{
	char *token = strtok(line, " \n");
	while (token) {
		*command++ = token;
		token = strtok(NULL, " \n");
	}
	*command = (char *)'\0';
}

void execute_commands (char **command)
{
	int pid;		// pid for the child process
	int status;

	if ((pid = fork()) < 0) {
		// Handle forking error
		printf("forking failed");
		exit(1);
	} else if (pid == 0) {
		// Execution withing child process
		if (execvp(*command, command) < 0 ) {
			// The if statements runs the command itself
			// so if it returns at all, there's an error
			printf("there was an error with the exec command");
			exit(1);
		}
	} else {
		while(wait(&status) != pid)
			// Wait for child exec process to do its thing
			;
		printf("Child process finished with id: %d\n", pid);
	}
}

int main(void)
{
	char line[1024];
	char *token;
	char *command[64];
	int i = 0;
	int status;
	pid_t pid;

	printf("prompt ~> ");
	while (fgets(line, sizeof(line), stdin)) {
		parse_interactive(line, command);

		execute_commands(command);
		// if ((pid = fork()) == 0) {
		// 	printf("made it to the fork\n");
		// 	printf("command %s\n", command[0]);
		// 	printf("command2 %s\n", command[1]);

		// 	if (execvp(*command, command) < 0) {
		// 		printf("there was an error\n");
		// 		exit(1);
		// 	} else {
		// 		printf("command supposedly ran\n");
		// 	}
		// } else {
		// 	while (wait(&status) != pid)
		// 		;
		// }
		// i = 0;
		printf("prompt ~> ");
	}
	return 0;
}
