#include <stdio.h>
#include <paths.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define SIZE1 50
//=============================================================================
// help function:

void freeArray (char** arr , int size){
    for(int i = 0 ;i < size ; i++){// free array
        free(arr[i]);
    }
    free(arr);
}


//=============================================================================
//split array function:
char* splitArrLeft (char* input , char* s){
    char c = s[0];
    int i = 0;
    while (input[i] != c){
        i++;
    }
    if (input[i-1] == ' ')
        i--;
    char* left = (char*)malloc(i*sizeof(char));
    for (int j = 0 ; j < i ; j++)
        left[j] = input[j];
    left[i] = '\0';
    return left;
}
char* splitArrRight (char* input , char * s){
    int k = 0;
    int i = 0;
    char c = s[0];
    while (input[i] != c){
        i++;
    }
    if (input[i+1] == ' ')
        i++;
    if(strlen(s) > 1)
        if(input[i] == s[1])
            i++;
    char* right = (char*)malloc((strlen(input) - i) * sizeof(char));
    for (int j = i+1 ; j < strlen(input) ; j++){
        right[k] = input[j];
        k++;
    }
    return right;
}


//================================================================================
// function to count words in string
int howManyWords(char* arr){ 
    int words = 0;
    int i = 0;
    while(arr[i] != '\0'){
        if(arr[i] != ' ' && arr[i+1] == ' ' ){ // words seperated by space
            words++;
        }
        i++;
        }
    return words+1;
}


//==============================================================================
// convert string (char*) to an array of words (char**)
char** sentenceToWordArray(char* sentence){ 
    char* temp =(char*)malloc((strlen(sentence)) * sizeof(char));
    strcpy(temp, sentence);
    int wordNum = howManyWords(sentence);
    int i = 0;
    char** toReturn;
    toReturn = (char**)malloc((wordNum+1) * sizeof(char*));// dynamic assignment of the words array 
    if(toReturn == NULL){ //if malloc didnt work
        perror("ERROR:memory allocation failed");
        exit(1);
    }
    char* temp1 = strtok(temp," ");
    toReturn[i]=(char*)malloc((strlen(temp)+1)*sizeof(char));
    if (toReturn[i]==NULL){ ///if malloc didnt work
        perror("ERROR:memory allocation failed");
        exit(1);
    } 
    strcpy(toReturn[i],temp1);
    i++;
    for (i = 1 ; i < wordNum ; i++){
        temp1 = strtok(NULL," ");
        toReturn[i]=(char*)malloc((strlen(temp1)+1)*sizeof(char));
        if(toReturn[i]==NULL){//if malloc didnt work
            puts("ERROR:memory allocation failed");
            exit(1);
        } 
        strcpy(toReturn[i],temp1);
    }
    toReturn[i] = NULL;
    return toReturn;
}


// shell function:


//==============================================================================
// function to print the prompt line of the shell:
void printPrompt(){ 
     char path[SIZE1];
     getcwd(path, SIZE1); // get the path of the current folder
     if(path == NULL){
        perror("ERROR");
     }   
     struct passwd * userInfo;
     if(userInfo == NULL){
        perror("ERROR");
     }   
     userInfo = getpwuid(getuid()); // get the user name
     printf("%s@%s>", userInfo->pw_name , path);    
}


//===========================================================================
//redirection functions:


//redirection >>:
void redirection1(char** left , char** right){
    int dupVal;
    int fd = open (right[0] , O_WRONLY | O_CREAT | O_APPEND , S_IRWXU);
    if(fd == -1){ //if open failed
        perror("cannot open file");
        exit(1);
    }
    dupVal = dup2(fd, STDOUT_FILENO);
    if(dupVal ==  -1){//if dup2 failed
        perror("dup2 failed\n");
        exit(1);
    }
    
    execvp( left[0] , left);
    printf("%s: command not found\n",left[0]);
    exit(1);//exec failed 
                           
}


// redirection 2> :
void redirection2(char **left, char **right){
  int dupVal;
  int fd = open (right[0], O_WRONLY|O_CREAT, S_IRWXU);
    if(fd == -1){//if open failed
        perror("cannot open file");
        exit(1);
    }
    dupVal = dup2(fd, STDERR_FILENO);
    if(dupVal ==  -1){ // if dup2 failed
        perror("dup2 failed\n");
        exit(1);
    }
    execvp(left[0],left);
    printf("%s: command not found\n",left[0]);
    exit(1);//exec failed                    
}


// redirection >:
void redirection3(char** left, char** right){  
    int dupVal;
    int fd = open(right[0], O_WRONLY|O_CREAT, S_IRWXU);
    if(fd == -1){ // if open failed
        perror("cannot open file");
        exit(1);
    }
    dupVal = dup2(fd, STDOUT_FILENO);
    if(dupVal ==  -1){//if dup2 failed
        perror("dup2 failed\n");
        exit(1);
    }                       
    execvp(left[0],left);
    printf("%s: command not found\n",left[0]);
    exit(1);//exec failed                         
}


// redirection <:
void redirection4(char **left, char **right){
    int dupVal;
    int fd = open(right[0] , O_RDONLY | O_CREAT , S_IRWXU);
    if(fd == -1){//if open failed
        perror("cannot open file");
        exit(1);
    }
    dupVal = dup2(fd, STDIN_FILENO);
    if(dupVal ==  -1){ //if dup2 failed
        perror("dup2 failed\n");
        exit(1);
    }
    execvp(left[0],left);
    printf("%s: command not found\n",left[0]);
    exit(1);//exec failed                         
}


//==============================================================================
// pipe and after redirection function:


// pipe and then redirection >:
void pipeRed1(char **left, char **right, int *pipe_fd){ 
    int val;
    int val2;
    int fd = open (right[0] , O_WRONLY | O_CREAT , S_IRWXU);
    if(fd == -1){ //if open failed
        perror("cannot open file");
        exit(1);
    }
    close(pipe_fd[1]);
    val = dup2(pipe_fd[0], STDIN_FILENO); 
    if(val ==  -1) {//if dup2 failed
        perror("dup2 failed\n");
        exit(1);
    }
    close(pipe_fd[0]);
    val2 = dup2(fd, STDOUT_FILENO);
    if(val2 ==  -1){//if dup2 failed
        perror("dup2 failed\n");
        exit(1);
    }                          
    execvp(left[0],left);
    printf("%s: command not found\n",left[0]);
    exit(1);//exec failed 
}


//pipe and redirection "2>":
void pipeRed2(char** left, char** right, int* pipe_fd){
    int dupVal;
    int dupVal1;
    int fd = open (right[0], O_WRONLY | O_CREAT , S_IRWXU);
    if(fd == -1){ //if open failed
    perror("cannot open file");
    exit(1);
    }
    close(pipe_fd[1]);
    dupVal = dup2(pipe_fd[0], STDIN_FILENO); 
    if(dupVal ==  -1){//if dup2 failed
        perror("dup2 failed\n");
        exit(1);
    }
    close(pipe_fd[0]);
    dupVal1 = dup2(fd, STDERR_FILENO);
    if(dupVal1 ==  -1){//if dup2 failed
        perror("dup2 failed\n");
        exit(1);
    }                               
    execvp(left[0],left);                              
    printf("%s: command not found\n",left[0]);
    exit(1);//exec failed 
}


// pipe and redirection >>:
void pipeRed3(char** left , char** right , int* pipe_fd){
    int dupVal;
    int dupVal1;
    int fd = open (right[0], O_RDWR | O_CREAT | O_APPEND , S_IRWXU);
    if(fd == -1){ //if open failed
        perror("cannot open file");
        exit(1);
    }
    close(pipe_fd[1]);
    dupVal = dup2(pipe_fd[0], STDIN_FILENO); 
    if(dupVal ==  -1){//if dup2 failed
    perror("dup2 failed\n");
    exit(1);
    }
    close(pipe_fd[0]);
    dupVal1 = dup2(fd, STDOUT_FILENO);
    if(dupVal1 ==  -1){//if dup2 failed
        perror("dup2 failed\n");
        exit(1);
    }
    execvp(left[0],left);                            
    printf("%s: command not found\n",left[0]);
    exit(1);//exec failed 
}


//============================================================================
//==============================================================================
//main function:
#define SIZE 150 // max length of command
 int main(int argc , char* argv[]){
     //instalizing:
    int commandCount = 1; // how many commands were used. starting from 1 since "done" command allways used
    int commandLength = 4;// the total length of the command that were used. starting from 4 since "done" command allways used
    int redirectionCount = 0;
    int pipeCount = 0;
    char input[SIZE];//an array to recive input from the user
    char** command;// an array to use in execvp function
    int pipeFd[2];
    int dupVal;
    pid_t leftId;
    pid_t rightId;
    char** right;
    char** left;
    char** redLeft;
    char** redRight;
    int ffd = open (argv[1], O_CREAT | O_WRONLY | O_TRUNC , S_IRWXU); 
    if( ffd == -1){ //if open failed
        perror("cannot open file");
        exit(1);
    }

    while(strcmp(input , "done")!= 0){//continue recive commands until the user enter the command "done"

        printPrompt(); // print the prompt line
        fgets(input , SIZE , stdin); // get input
        commandCount++;
        commandLength += (strlen(input));
        
        if ( strcmp( input , "\n") == 0)// if no command was entered
            continue;
        input[strlen(input)-1]='\0'; // overwrite the '\n' char at the end
        if(input[strlen(input)-1] == ' ')
            input[strlen(input)-1]='\0';
        
        // if there is pipe | do:
        if((strstr(input, "|") != NULL) ){ 
            pipeCount++;
            char * temp = splitArrLeft(input , "|");
            left = sentenceToWordArray(temp);
            write (ffd, left[0] , strlen(left[0]));
            write (ffd, "\n", 2);
            
            if(pipe(pipeFd) == -1){ // if pipe didnt work
                perror("pipe failed");
                exit(1);
            }
            leftId = fork();
            if(leftId < 0){ ///if fork didnt work
                perror("ERROR: could not open a new procces\n");
                exit(1);
            }
            if(leftId == 0){ // left child procces
                close(pipeFd [0]);
                dupVal = dup2(pipeFd[1], STDOUT_FILENO);
                if(dupVal ==  -1){
                    perror("dup2 failed\n");
                    exit(1);
                }
                close(pipeFd[1]);
                //write (ffd, left[0], strlen(left[0]));
                execvp(left[0],left);
                perror("command not found\n");
                exit(1);//exec failed 
            }
            rightId = fork();
            if(rightId < 0){ ///if fork didnt work
                perror("ERROR: could not open a new procces\n");
                exit(1);
            }
            char* temp1 = splitArrRight(input , "|");
            right = sentenceToWordArray(splitArrRight(input , "|"));
            
            if(rightId == 0){ // right child procces             
            write (ffd, right[0] , strlen(right[0]));
            write (ffd, "\n", 2);
            //pipe and redirection:
            if((strstr(input, ">") != NULL)||(strstr(input, "<") != NULL)){
                redirectionCount++;
                if ((strstr(input , "2>") != NULL)){
                    redLeft = sentenceToWordArray(splitArrLeft(splitArrRight(input , "|") , "2>"));
                    redRight = sentenceToWordArray(splitArrRight(splitArrRight(input , "|") , "2>"));
                    pipeRed2(redLeft ,redRight ,pipeFd);
                    freeArray(redRight , howManyWords(splitArrRight(splitArrRight(input , "|") , "2>")));
                    freeArray(redLeft , howManyWords(splitArrLeft(splitArrRight(input , "|") , "2>")));
                }
                 
                else if ((strstr(input , ">>"))){
                    redLeft = sentenceToWordArray(splitArrLeft((splitArrRight(input , "|")) , ">>"));
                    redRight = sentenceToWordArray(splitArrRight(splitArrRight(input , "|") , ">>"));  
                    pipeRed3(redLeft ,redRight ,pipeFd);
                    freeArray(redRight , howManyWords(splitArrRight(splitArrRight(input , "|") , ">>")));
                    freeArray(redLeft , howManyWords(splitArrLeft(splitArrRight(input , "|") , ">>")));
                }
                else if((strstr(input , ">") != NULL)){
                    redLeft = sentenceToWordArray(splitArrLeft(splitArrRight(input , "|") , ">"));
                    redRight = sentenceToWordArray(splitArrRight(splitArrRight(input , "|") , ">"));
                    pipeRed1(redLeft ,redRight ,pipeFd);
                    freeArray(redRight , howManyWords(splitArrRight(splitArrRight(input , "|") , ">")));
                    freeArray(redLeft , howManyWords(splitArrLeft(splitArrRight(input , "|") , ">")));
                 }   
            }
            else{//pipe without redirection:
                close(pipeFd[1]);
                dupVal = dup2(pipeFd[0] , STDIN_FILENO);
                if(dupVal ==  -1){//if dup2 failed
                   perror("dup2 failed\n");
                    exit(1);
                }
                close(pipeFd[0]);
                execvp(right[0],right); 
                printf("command not found\n");
                exit(1);//exec failed 
            }
        }
        close(pipeFd[0]);   
        close(pipeFd[1]);
        wait(NULL);
        wait(NULL);
        freeArray(right , howManyWords(splitArrRight(input , "|")));
        freeArray(left , howManyWords(splitArrLeft(input , "|")));
        continue;
        }
        //if there is redirection do:
        else if((strstr(input , ">") != NULL)||(strstr(input , "<") != NULL)){
            redirectionCount++;
            int id = fork();
            if (id < 0){ //if fork failed
                perror("ERROR: could not open a new procces\n");
                exit(1);
            }
            command = sentenceToWordArray(input);
            write (ffd, command[0] , strlen(command[0]));
            write (ffd, "\n", 2);

            if( id == 0){// child procces
            if((strstr(input , ">>") != NULL)){//redirection is '>>'                           
                left = sentenceToWordArray(splitArrLeft (input , ">>"));
                right = sentenceToWordArray(splitArrRight (input , ">>"));                             
                redirection1(left,right);
                freeArray(right , howManyWords(splitArrRight(input , ">>")));
                freeArray(left , howManyWords(splitArrLeft(input , ">>")));      
            }

            else if((strstr(input , "2>") != NULL)){ // redirection  '2>'
                left = sentenceToWordArray(splitArrLeft (input , "2>"));
                right = sentenceToWordArray(splitArrRight (input , "2>")); 
                redirection2(left,right);
                freeArray(right , howManyWords(splitArrRight(input , "2>")));
                freeArray(left , howManyWords(splitArrLeft(input , "2>")));                         
            }

            else if((strstr(input , ">") != NULL)){// redirection '>'
                left = sentenceToWordArray(splitArrLeft (input , ">"));
                right = sentenceToWordArray(splitArrRight (input , ">")); 
                redirection3(left,right);
                freeArray(right , howManyWords(splitArrRight(input , ">")));
                freeArray(left , howManyWords(splitArrLeft(input , ">")));  
                }

            else if((strstr(input , "<") != NULL)){// redirection '<'
                left = sentenceToWordArray(splitArrLeft (input , "<"));
                right = sentenceToWordArray(splitArrRight (input , "<"));  
                redirection4(left,right);
                freeArray(right , howManyWords(splitArrRight(input , "<")));
                freeArray(left , howManyWords(splitArrLeft(input , "<")));  
                }

            }
        wait(NULL);
        continue;
        }
        //no pipe nor redirection:
        command = sentenceToWordArray(input);
        if (strcmp( command[0] , "cd") == 0){ // cd not supported
            printf("commend not supported (yet)\n");
            continue;
        }
        int id = fork();
        if (id == -1){ //if fork failed
            perror("ERROR: could not open a new procces\n");
            exit(1);
        }  
        if(id == 0){ //in the child process
            write (ffd, command[0] , strlen(command[0]));
            write (ffd, "\n", 2);
            execvp(command[0],command);
            if(strcmp( command[0] , "done") != 0){
                printf("%s: command not found\n",command[0]);
                exit(1);
            }
        }
        wait(NULL); // wait for the child process to finish
        freeArray(command , howManyWords(input));
    }

    //the end lines:
    printf("Num of commands: %d\n", commandCount);
    printf("Total length of all commands: %d\n", commandLength);
    printf("Average length of all commands: %f\n", ((double)commandLength/commandCount));
    printf("See you Next time!\n");
    printf("Number of command that include pipe: %d\n", pipeCount);
    printf("Number of command that include redirection: %d\n", redirectionCount);
    return 0;
}
