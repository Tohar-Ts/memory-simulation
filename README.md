
The program implement a simple  shell in c language under the operating system.
the program show the user the prompt line, in pattern of "(user name)@(current folder path)>".
after the prompt line, the shell wait for the user to insert command and maybe arguments.
the program translates the command that was insert as a string to an array that can be sent to the operating system,
than the progrem sends the command and the arguments to the operating system to run the command.
I added to the progrem the option to use pipe and redirection.
pipe: send the outputfrom one command to the input of another command. use:  firstCmd | secondCmd
redirection: send the output from the command to a file or take input from file. use: "command" >/ >>/ 2>/ < file name
> send output and overwrite the file
>>send output and don't overwrite the file
2> send error output
< recive from file
the progrem get file log.txt that save all the command the user insert.


The  programs input and output:

1) after the prompt line is display , the user insert the command the user want to run
2) after each command the user will type enter to execute the command or type arguments diveratde by space and than enter.
the progrem dont support the cd command, if the user try to execute cd command he get a proper message (command not supported yet).
3) the prompt line is display again and the user can run another command
The commands will be sent to the operating system for execution using the commands fork and execvp. 
the program create a child process, and the child process uses the execvp command to run the command the user insert.
4) when the user wants to end the shell, he uses the command "done"
5) at the end, the next 4 lines will be printed on the screen:
	1 - the first line shows how much commands the user enter (including the command "done").
	2 - the second line prints the total length of all commands that the user enter.
	3 - the third line shows the average length of the commands.
	4 - the fourth line prints "see you next time!".

help function in the program:
howManyWords: function to count words in string
sentenceToWordArray: function to convert string (char*) to an array of words (char**)
printPrompt: function to print the prompt line of the shell
freeArray: to free char** array
splitArrRight / splitArrLeft : to split an array to 2 parts diveratde by const char* (string) 
	
To compile the program:

we need to press ctrl+shift+b and than press enter.
make sure to send log.txt file in "arg" in launch.jason file.
in the terminal- gcc ex1.c –o ex1 
To run the program:
we need to press ctrl+f5.	
in terminl- after we copmpile the file we need to write ./ex2 log.txt.