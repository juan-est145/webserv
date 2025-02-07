#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(void)
{
	pid_t pid;
	int fd[2];
	int status;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		char *args[] = {
			(char *)"python3",
			(char *)"pyTest.py",
			NULL,
		};
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		// char *env[] = {
		// 	{ "python3" },
		// 	{ "pyTest.py "},
		// 	{ NULL },
		// };
		execve((const char *)"/usr/bin/python3", args, NULL);
		exit(0);
	}
	else
	{
		close(fd[1]);
		char buffer[600];
		waitpid(pid, &status, 0);
		read(fd[0], buffer, 13);
		close(fd[0]);
		int resultFd = open("./result.txt", O_CREAT | O_WRONLY, S_IRWXU);
		write(resultFd, buffer, 13);
	}
		
	return (0);
}