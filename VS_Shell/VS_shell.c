/******************************************************************/
/*******************Simple Command line interpreter****************/
/******************************************************************/
/*README:
 *VS_Shell
 *Authors-
 *	Saumya Shah
 *	Varsha Teckchandani
 *VS_Shell is a command line interpreter which supports all the basic commands which do not include a change of the directory. 
 *VS_Shell has an additional feature to save the command to some file so that file can be run as a script. 
 *VS_Shell also has options to change the color of the shell. 
 *To make a new file -
 *	Type the command 'newfile' and then enter the name of the file. 
 *	All the commands which are typed now will be saved in that file. 
 *To write  a command which should not go in the file-
 *	'command' <space> ;
 *	Example-  On writing ls ; the output will be shown but ls command wont be written in the file. 
 *To exit the file-
 *	'closefile' 
 *To change the color-
 *	Type 'color' on the VS_shell. Then choose from the given options. 
 *VS_Shell also supports the pipeline function.
 *	Example-  ls | grep shell | wc
 *
 *For further queries contact-
 *	saumya.shah295@gmail.com
 *	varshat254@gmail.com
 */
/* Compile with: cc VS_shell.c -o VS.out */
/* Execute with: ./VS.out */
/******************************************************************/
/******************************************************************/
/******************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define ERROR(fmt, arg) return fprintf(stderr, fmt "\n", arg)

/* The array below will hold the arguments: args[0] is the command. */
static char *args[512];
pid_t pid;
int command_pipe[2];
int icnt = 1, file_flag = 0, flag = 0, temp1 = 0;
char fname[50] = "";
FILE *fptr;
static int run (char *cmd, int input, int first, int last);
char line[1024], temp[1024];
static int n = 0;		/* number of calls to 'command' */

#define READ  0
#define WRITE 1

/*
 *Input: Filename,num_of_lines to be deleted
 *Counts the number of lines of input file and then deletes the last input line
 */

int
del_line (char fname[50], int num_of_lines)
{
  FILE *fp;
  char *buf;
  size_t sz;
  int start, count, lines = 1;
  int line_index = 0;
  int linecount = 0;
  int dest = 0, src = 0, pos = -1;
  char temp;
  //Open the file 
  fp = fopen (fname, "r");
  //Get a character from the file
  temp = getc (fp);
  //Count the number of lines in the file 
  while (temp != EOF)
    {
      if (temp == '\n')
	linecount = linecount + 1;
      temp = getc (fp);
    }
  line_index = linecount;
  //Initialization
  count = num_of_lines;
  start = line_index;
  /* Determine filesize and allocate a buffer accordingly. */
  fseek (fp, 0, SEEK_END);
  sz = ftell (fp);
  buf = malloc (sz + 1);
  rewind (fp);
  /* Fill the buffer, count lines, and remember a few important offsets. */
  while ((buf[++pos] = fgetc (fp)) != EOF)
    {
      if (buf[pos] == '\n')
	{
	  ++lines;
	  if (lines == start)
	    dest = pos + 1;
	  if (lines == start + count)
	    src = pos + 1;
	}
    }
  /* Overwrite the lines to be deleted with the survivors. */
  memmove (buf + dest, buf + src, pos - src);
  /* Reopen the file and write back just enough of the buffer. */
  freopen (fname, "w", fp);
  fwrite (buf, pos - src + dest, 1, fp);
  free (buf);
  fclose (fp);
  return 0;
}

/*
 * Handle commands separatly
 * input: return value from previous command (useful for pipe file descriptor)
 * first: 1 if first command in pipe-sequence (no input from previous pipe)
 * last: 1 if last command in pipe-sequence (no input from previous pipe)
 *
 * EXAMPLE: If you type "ls | grep shell | wc" in your shell:
 *    fd1 = command(0, 1, 0), with args[0] = "ls"
 *    fd2 = command(fd1, 0, 0), with args[0] = "grep" and args[1] = "shell"
 *    fd3 = command(fd2, 0, 1), with args[0] = "wc"
 *
 * So if 'command' returns a file descriptor, the next 'command' has this
 * descriptor as its 'input'.
 */

static int
command (int input, int first, int last)
{
  int pipettes[2];
  int color;
  /* Invoke pipe */
  pipe (pipettes);
  // Fork new process
  pid = fork ();
  // If pid==0 i.e. it is a parent process
  if (pid == 0)
    {
      if (first == 1 && last == 0 && input == 0)
	{
	  // First command
	  dup2 (pipettes[WRITE], STDOUT_FILENO);
	}
      else if (first == 0 && last == 0 && input != 0)
	{
	  // Middle command
	  dup2 (input, STDIN_FILENO);
	  dup2 (pipettes[WRITE], STDOUT_FILENO);
	}
      else
	{
	  // Last command
	  dup2 (input, STDIN_FILENO);
	}
      //If user input command is 'color'
      if (strcmp (args[0], "color") == 0)
	{
	  //Ask to choose the color for text
	  printf
	    ("1. Default\n2. Black\n3. Red\n4. Green\n5. Yellow\n6. Blue\n7. Magenta\n8. Cyan\n9. Gray\n");
	  printf
	    ("\nEnter number for the color of choice from the list below:");
	  scanf ("%d", &color);
	  //Change the color of the text as per user choice
	  if (color == 1)
	    {
	      //Change the text color to Default (White)
	      char *arg[] = { "echo", "-e", "\e[39m", NULL };
	      execvp (arg[0], arg);
	    }
	  else if (color == 2)
	    {
	      //Change the text color to  Black
	      char *arg[] = { "echo", "-e", "\e[30m", NULL };
	      execvp (arg[0], arg);
	    }
	  else if (color == 3)
	    {
	      //Change the text color to Red
	      char *arg[] = { "echo", "-e", "\e[31m", NULL };
	      execvp (arg[0], arg);
	    }
	  else if (color == 4)
	    {
	      //Change the text color to Green
	      char *arg[] = { "echo", "-e", "\e[32m", NULL };
	      execvp (arg[0], arg);
	    }
	  else if (color == 5)
	    {
	      //Change the text color to Yellow
	      char *arg[] = { "echo", "-e", "\e[33m", NULL };
	      execvp (arg[0], arg);
	    }
	  else if (color == 6)
	    {
	      //Change the text color to Blue
	      char *arg[] = { "echo", "-e", "\e[34m", NULL };
	      execvp (arg[0], arg);
	    }
	  else if (color == 7)
	    {
	      //Change the text color to Blue
	      char *arg[] = { "echo", "-e", "\e[35m", NULL };
	      execvp (arg[0], arg);
	    }
	  else if (color == 8)
	    {
	      //Change the text color to Magenta
	      char *arg[] = { "echo", "-e", "\e[36m", NULL };
	      execvp (arg[0], arg);
	    }
	  else if (color == 9)
	    {
	      //Change the text color to Cyan
	      char *arg[] = { "echo", "-e", "\e[90m", NULL };
	      execvp (arg[0], arg);
	    }
	  else
	    {
	      //Change the text color to Gray
	      printf ("Invalid Color choice\n");
	      _exit (EXIT_FAILURE);
	    }
	}
      // Execute the command. If execution is unsuccessfull execvp returns -1 
      if (execvp (args[0], args) == -1 && strcmp (args[0], "newfile") != 0
	  && strcmp (args[0], "color") != 0)
	{
	  printf ("**** Error ****\n");
	  // if file has been created delete the last input line which was not successfully executed
	  if (fptr != NULL)
	    {
	      del_line (fname, 1);
	    }
	  //Exit the child process  
	  _exit (EXIT_FAILURE);
	}
    }
  if (input != 0)
    close (input);
  // Nothing more needs to be written
  close (pipettes[WRITE]);
  // If it's the last command, nothing more needs to be read
  if (last == 1)
    close (pipettes[READ]);

  return pipettes[READ];
}

/* Final cleanup, 'wait' for processes to terminate.
 *  n : Number of times 'command' was invoked.
 */
static void
cleanup (int n)
{
  int i;
  for (i = 0; i < n; ++i)
    wait (NULL);
}


static char *
skipwhite (char *s)
{
  while (isspace (*s))
    ++s;
  return s;
}

/*
 * Split the input commands that are given as an input with pipes
 */
static void
split (char *cmd)
{
  //Skip white spaces that are left before and after the pipe
  cmd = skipwhite (cmd);
  //Pointer to detect space
  char *next = strchr (cmd, ' ');
  int i = 0;
  //Nullify all the white spaces and point the next to the next valid character
  while (next != NULL)
    {
      next[0] = '\0';
      args[i] = cmd;
      ++i;
      cmd = skipwhite (next + 1);
      next = strchr (cmd, ' ');
    }

  if (cmd[0] != '\0')
    {
      args[i] = cmd;
      next = strchr (cmd, '\n');
      next[0] = '\0';
      ++i;
    }
  args[i] = NULL;
}

/*
 * Checks for specific commands and lets others to run
 * If "exit command is detected then the shell is made to exit
 * If "newfile" command is detected then a newfile is created and user is asked the name of the file to be created
 * If "closefile" is detected then the shell exits the file in which it was writing 
 * For all other input commands the input is passed on to the function "command" to execute the input value
 */
static int
run (char *cmd, int input, int first, int last)
{
  split (cmd);
  // Check if the input argument is not null i.e. user has given a not null argument as an input to the shell
  if (args[0] != NULL)
    {
      // Check if the user input command is "exit" then exit the shell
      if (strcmp (args[0], "exit") == 0)
	{
	  printf ("EXIT\n");
	  file_flag = 0;
	  char *arg[] = { "echo", "-e", "\e[39m", NULL };
	  execvp (arg[0], arg);
	  exit (1);
	}

      // Check if the user input command is "newfile" then create a new file    
      else if (strcmp (args[0], "newfile") == 0 && fptr == NULL)
	{
	  // Input filename
	  printf ("Enter filename:");
	  scanf ("%s", fname);
	  // Open the file
	  fptr = fopen (fname, "w+");
	  // If the file hasn't been created successfully
	  if (fptr == NULL)
	    {
	      printf ("Error!");
	      exit (1);
	    }
	  flag = 2;
	  temp1 = 1;
	  file_flag = 1;
	}
      // Check if the user input command is "closefile" then close the file which is open               
      else if (strcmp (args[0], "closefile") == 0 && fptr != NULL)
	{
	  file_flag = 0;
	  // Delete the first line of the file which is a null line
	  del_line (fname, 1);
	  fptr = NULL;
	  printf ("Closing the file\n");
	}
      else
	{
	  n += 1;
	  return command (input, first, last);
	}
    }
  return 0;
}


int
main ()
{
  char choice;
  while (1)
    {
      /* Print the command prompt */

      if (flag == 0)
	{
	  printf ("\n\t\t\033[36mWELCOME TO MY SHELL!!!\033[0m\n\n");
	  flag = 1;
	}
      if (flag == 1 && fptr == NULL)
	printf ("VS_SHELL:~$> ");
      else if (flag == 1 && fptr != NULL && temp1 == 0)
	printf ("VS_SHELL:~/%s$>", fname);
      else
	temp1 = 0;
      fflush (NULL);

      /* Read a command line */
      if (!fgets (line, 1024, stdin))
	return 0;

      int input = 0;
      int first = 1;
      char *cmd = line;
      char *next = strchr (cmd, '|');	/* Find first '|' */

      while (next != NULL)
	{
	  /* 'next' points to '|' */
	  *next = '\0';
	  input = run (cmd, input, first, 0);
	  cmd = next + 1;
	  next = strchr (cmd, '|');	/* Find next '|' */
	  first = 0;
	}
      char *next1 = strchr (cmd, ';');	/* Find  ';' */
      //Open the file if fname is not null
      if (fname != '\0' && file_flag == 1)
	{
	  fptr = fopen (fname, "a");
	}

      if (fptr != NULL)
	{
	  //If file is open and ";" is observed in the parsed arguement then replace it by a null character
	  if (next1 != NULL)
	    {
	      *next1 = ' ';
	    }
	  //If file is open and ";" is not observed in the parsed arguement then write the command to the file  
	  else
	    {
	      fprintf (fptr, "%s", line);
	    }
	}
      //If the file is open, close it 
      if (fptr != NULL)
	{
	  fclose (fptr);
	}
      // Run the command that is parsed as the input  
      input = run (cmd, input, first, 1);
      if (flag == 2)
	flag = 1;
      //Wait till all commands are executed
      cleanup (n);
      n = 0;
    }

  return 0;
}
