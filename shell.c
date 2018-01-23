#include <stdio.h>
#include <unistd.h>

int main(void)
{	
	char line[1024];
	char *argv[64];
	pid_t pid;

	printf("prompt ~> ");
	while (fgets(line, sizeof(line), stdin)) {
		printf("you typed: %s\n", line);
		if (pid = fork() == 0) {
			execvp(*line, line);
		}
		printf("prompt ~> ");
	}
	return 0;
}
