/* Creating My_Shell as our own shell and taking inputs from user as char strings. Detecting "Enter Key" and "Ctrl+D" */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
	char c = '\0';
	printf("\n[MY_SHELL ] ");
	while(c != EOF) {
		c = getchar();
		if(c == '\n')
			printf("[MY_SHELL ] ");
	}
	printf("\n");
	return 0;
}