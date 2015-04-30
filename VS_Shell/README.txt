*******
README: 
VS_Shell
*******

***********************************************************************************************************
Authors-
Varsha Teckchandani
Saumya Shah
***********************************************************************************************************

***********************************************************************************************************
VS_Shell is a command line interpreter which supports all the basic commands which do not include a change of the directory. 
VS_Shell has an additional feature to save the command to some file so that file can be run as a script. 
VS_Shell also has options to change the color of the shell. 

To make a new file -
Type the command "newfile" and then enter the name of the file. 
All the commands which are typed now will be saved in that file. 

To write  a command which should not go in the file-
"command" <space> ;
Example-  On writing ls ; the output will be shown but ls command wont be written in the file. 

To exit the file-
closefile 

To change the color-
Type color on the VS_shell. Then choose from the given options. 

VS_Shell also supports the pipeline function.
Example-  ls | grep shell | wc
***********************************************************************************************************

***********************************************************************************************************
Compilation: cc VS_shell.c -o VS_shell.out
Execution:   ./VS_shell.out
***********************************************************************************************************

***********************************************************************************************************
For further queries contact-
varshat254@gmail.com
saumya.shah295@gmail.com
***********************************************************************************************************