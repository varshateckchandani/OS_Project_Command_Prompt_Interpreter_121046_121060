/* Running system call execve() to get output for ls command */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
	execve("/bin/ls", argv, envp);
	return 0;
}