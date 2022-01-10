/* Big Red Shell
 * Group Members: Joshua Mallari, Jennifer Tsan, Nicholas Wade
 * Course: COSC 4302 - Fall 2021
 * Instructor: Dr. Sun
 * Project: A Simple Shell
 */

#include    <stdio.h>
#include    <stdlib.h>
#include    <sys/types.h>
#include    <sys/wait.h>
#include    <string.h>
#include    <unistd.h>

// Read command lines from the file
struct command_t {
    char *name;
    int argc;
    char *argv[64];
};

char *lookupPath(char **, char **);
int parseCommand(char *, struct command_t *);
void parsePath(char **);
void printPrompt();
void readCommand(char *);
static int counter = 0;

char promptString[] = "BigRed>";
extern FILE *stdin;

int main()
{
    char commandLine[80];
    char *pathv[64];
    int numPaths;
    // for parent
    int stat;
    struct command_t command;

    // Shell
    for(int i=0; i<64; i++)
        command.argv[i] = (char *) malloc(16);

    parsePath(pathv);

    if(counter == 0) {
            printf("               z\n");
            printf("             z\"F\"$$.\n");
            printf("       -&- . Led$$$$P-\n");
            printf("              3$3 F3$&\n");
            printf("            \" ^  .3\"\"\n");
            printf("                 d***$$e.\n");
            printf("              r .&     ^\"&\n");
            printf("              '$$r\n");
            printf("               3$$  *$*$$$$$\n");
            printf("                '$$. *b'b\"$*$.\n");
            printf("                  *$. \"L^L\"b\"$-\n");
            printf("                   \"$b '. L^b^$\n");
            printf("                    ^$$bJ  \\ b^$ .\n");
            printf("                    b *$$$b.\\ \\ b \\\n");
            printf("                    *$.\"$$$$$b.. & &\n");
            printf("                    4$$r'$$b *$.&.\\ \".\n");
            printf("                    ^$$  $$P  \"$.'c^c\"e\n");
            printf("                    4P\"  $F&   '$.'c^r*$c\n");
            printf("                    $    $      '$.'c^c \"$-\n");
            printf("                   $&   .$       '$.'L^b\n");
            printf("           J$$$$$$$$$$$$$$$$$$     *.JL.b BigRed'\n");
            printf("===================Big Red: a simple C Shell===================\n");
            printf("===============================================================\n");
        }
        else{
            ++counter;
        }
    
    while(1)
    {
        //prints out prompt
        printPrompt();
        // reads user input in command line
        readCommand(commandLine);
        // parses user input from the command line
        parseCommand(commandLine, &command);

        //run exit, quit, or logout command
        if( (strcmp(commandLine, "exit") == 0) || (strcmp(commandLine, "quit") == 0) || (strcmp(commandLine, "logout") == 0)){
            // Quit the shell
            break;
        }else
            // run help command
            if(strcmp(commandLine, "help") == 0){
                printf("Commands: ls cd exit quit logout\n");
                continue;
            }
        // run cd command
        else if(strcmp(commandLine, "cd") == 0){
            if (command.argv[1] == NULL) {
                fprintf(stderr, "[BigRed: Expected Arguments to \"cd.\"]\n");
            }else {
                if (chdir(command.argv[1]) != 0) {
                    printf("[BigRed: File or Directory not found.]\n");
                }
            }
            continue;
        }
        
        // Get full path name
        command.name  = lookupPath(command.argv, pathv);
        
        if(command.name == NULL)
        {
            fprintf(stderr, "[Command \"%s\" not found]\n", command.argv[0]);
            continue;
        }
        
        // Process creation
        if(fork() == 0)
        {
            //  Child Process: executes the command
            execv(command.name, command.argv);
       }
       // wait for child process termination
        wait(&stat);
  }
// Shell termination
    return 0;
}

/* Look Up Path Function
 * This function searches the directories identified by the dir
 * argument to see if argv[O] (the file name) appears there.
 * Allocate a new string, place the full path name in it, then
 * return the string.
 */
char *lookupPath(char **argv, char **dir)
{
    char *result;
    char pName[96];

    // Checking if file name in path already
    if(*argv[0] == '/')
    {
        result = (char *) malloc(strlen(argv[0])+1);
        strcpy(result, argv[0]);
        return result;
    }

    // looking in directories
    for(int i = 0; i < 64; i++)
    {
        if(dir[i] == NULL) break;
        strcpy(pName, dir[i]);
        strcat(pName, "/");
        strcat(pName, argv[0]);

        if(access(pName, X_OK | F_OK) != -1)
        {
            result = (char *) malloc(strlen(pName)+1);
            strcpy(result, pName);
            return result;
        }
    }
    return NULL;
}

/* Parse Command Function
 * Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char*). The last
 * element in argv(] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[O] , the second in
 * argv[l], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd)
{
    // Determine command name and construct the parameter list
    int argc;
    char **clPtr;

    // Initialization
    clPtr = &cLine;
    argc = 0;

    // Get the command name and parameters
    // This code does not handle multiple WHITESPACE characters
    while((cmd->argv[argc++] = strsep(clPtr, " .,\t\n")) != NULL) ;

    //terminated list of strings
    cmd->argv[argc--] = NULL;
    cmd->argc = argc;

    return  1;
}

/* Parse Path Function
 * This function reads the PATH variable for this
 * environment, then builds an array, dirs[], of the
 * directories in PATH
 */
void parsePath(char *dirs[])
{
    char *pathEnvVar;
    register char *thePath, *oldp;

    for(int i=0; i<64; i++)
    dirs[i] = NULL;

    pathEnvVar = (char *) getenv("PATH");
    thePath = (char *) malloc(strlen(pathEnvVar) + 1);
    strcpy(thePath, pathEnvVar);

    int i = 0;
    oldp = thePath;

    for(;; thePath++)
    {
    if((*thePath == ':') || (*thePath == '\0'))
        {
            dirs[i] = oldp;
            i++;

            if(*thePath == '\0')
                break;

            *thePath = '\0';
            oldp = thePath + 1;
        }
    }
}

/* Print Prompt Function
 * prints out at the beginning of the each line
 */
void printPrompt()
{
    printf("%s ", promptString);
}

/* Read Command Function
 * This code uses any set of I/O functions, such as those in
 * the stdio library to read the entire command line into
 * the buffer. This implementation is greatly simplified,
 * but it does the job.
 */
void readCommand(char *buffer)
{
    fgets(buffer, 80, stdin);
    //overwrite the line feed with NULL term
    buffer[strlen(buffer)-1] = '\0';
}
