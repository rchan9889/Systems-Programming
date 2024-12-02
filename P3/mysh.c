#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <glob.h>
#include <sys/wait.h>
#include <fcntl.h>

char* where(char path[]) {
    char dirOne[15 + strlen(path) + 1];
    memset(dirOne, 0, strlen(path) + 1);
    strcat(dirOne, "/usr/local/bin/");
    strcat(dirOne, path);
                    
    char dirTwo[9 + strlen(path) + 1];
    memset(dirTwo, 0, sizeof(dirTwo));
    strcat(dirTwo, "/usr/bin/");
    strcat(dirTwo, path);
                    
    char dirThree[5 + strlen(path) + 1];
    memset(dirThree, 0, sizeof(dirThree));
    strcat(dirThree, "/bin/");
    strcat(dirThree, path);
    
    if (access(dirOne, F_OK) != -1) {
   	char* answerOne = malloc((15 + strlen(path) + 1) * sizeof(char));
	strcpy(answerOne, dirOne);
	return answerOne;
    } else if (access(dirTwo, F_OK) != -1) {
        char* answerTwo = malloc((9 + strlen(path) + 1) * sizeof(char));
        strcpy(answerTwo, dirTwo);
	return answerTwo;
    } else if (access(dirThree, F_OK) != -1) {
        char* answerThree = malloc((5 + strlen(path) + 1) * sizeof(char));
        strcpy(answerThree, dirThree);
	return answerThree;
    } else {
	return NULL;
    }
                   
}

int main(int arc, char *argv){
    int interactive;
    int fd;

    fd = open(argv, O_RDWR);

    if(fd == -1){ //no argument
        interactive = 1;
    }else if(!interactive && !isatty(fd)){
        interactive = 0;
    }else{
        interactive = 1;
    }

    //interactive mode
    if(interactive){
        printf("Welcome to my shell!\n");
        while(interactive){
            printf("mysh> ");
            fflush(stdout);
            char *command = malloc(200 * sizeof(char));

            if (read(0, command, 200) == 0) {
                printf("Something wrong has occured. Exiting program now.\n");
                return 1;
            } 

            if(command[0] == 'c' && command[1] == 'd' && command[2] == ' '){ //cd
                char path[PATH_MAX];
                int i = 3;
                while(command[i]){
                    if (command[i] != 10) {
                        path[i - 3] = command[i];
                    }
		    i++;
                }
                printf("%s\n", path);
		if(path[0] == 47) { // The ascii value for backslash
                    if(chdir(path) == -1){
                        printf("cd: No such file or directory\n");
                    }else{
                        char cwd[PATH_MAX];
                        getcwd(cwd, sizeof(cwd));
                        printf("%s\n", cwd);
                    }
	        }else{
		    char cwd[PATH_MAX];
                    getcwd(cwd, sizeof(cwd));
                    char *filename = malloc(sizeof(cwd) + 1 + sizeof(path) + 1); // blah/blah + / + testfile + \0
                    
	            strcat(filename, cwd);
                    strcat(filename, "/");
                    strcat(filename, path);

                    if(chdir(filename) == -1){
                        printf("cd: No such file or directory\n");
                    }else{
                        char cwds[PATH_MAX];
                        getcwd(cwds, sizeof(cwds));
                        printf("%s\n", cwds);
                    }
                    free(filename);
                }
                memset(path, 0, sizeof(path));
            }else if(command[0] == 'p' && command[1] == 'w' && command[2] == 'd'){ //pwd
                char cwd[PATH_MAX];
                getcwd(cwd, sizeof(cwd));
                printf("%s\n", cwd);
		memset(cwd, 0, sizeof(cwd));
            }else if(command[0] == 'w' && command[1] == 'h' && command[2] == 'i' && command[3] == 'c' && command[4] == 'h' && command[5] == ' '){ //which
        	char path[PATH_MAX];
                memset(path, 0, sizeof(path));
                int i = 6;

                while(command[i]){
                    if (command[i] != 10) {
                        path[i - 6] = command[i];
		    }
		    i++;
                }

                char* result = where(path);

                if (result == NULL) {
                    printf("%s wasn't found in any of the three directories.\n", path);
                }
                else {
                    printf("%s\n", result);
                }

                free(result);
            }else if(command[0] == 'e' && command[1] == 'x' && command[2] == 'i' && command[3] == 't' && command[4] <= 32){ //exit
                printf("exiting");
                int i = 4;
                while(command[i] != '\0'){
                    printf("%c", command[i]);
                    i++;
                }

                return 0;
            }else{ //bare names & path names
                int i = 0;
                int j = 0;
                int k = 0;
                char arguments[10][40];
                memset(arguments, 0, sizeof(arguments));
                
                while(command[i] != '\0'){
                    if(command[i] == ' '){
                        k = 0;
                        j++;
                    }else{
                        arguments[j][k] = command[i];
                        k++;
                    }
                    i++;
                }
                
                char exec[PATH_MAX];
                memset(exec, '\0', sizeof(exec));
                char **args = malloc(9 * sizeof(char *));
                args[0] = malloc(9 * 40 * sizeof(char));

                for(int i = 1; i < 9; i++){
                    args[i] = args[0] + i * 40;
                }

                int x = 1;

                for(int l = 0; l < 9; l++){
                    if(l == 0){
                        if(arguments[l][0] == '.' && arguments[l][1] == '/'){
                            char file[PATH_MAX];
                            int i = 1;

                            while(arguments[l][i] != 10){
                                file[i - 1] = arguments[l][i];
                                i++;
                            }

                            getcwd(exec, (sizeof(exec)));
                            strcat(exec, file);
                            exec[strlen(exec)] = '\0';
                        }else{
                            if(arguments[l][0] != '/'){
                                if(arguments[l][strlen(arguments[l]) - 1] != 10){
                                    arguments[l][strlen(arguments[l])] = 0;
                                }else{
                                    arguments[l][strlen(arguments[l]) - 1] = 0;
                                }
                                sprintf(exec, "%s", where(arguments[l]));
                            }else{
                                sprintf(exec, "%s", arguments[l]);
                            }
                        }

                        sprintf(args[0], "%s", exec);
                    }else{
                        for(int m = 0; m < strlen(arguments[l]); m++){
                            if(arguments[l][m] == '*'){
                                glob_t globbuf;

                                char *wildcard = malloc(strlen(arguments[l]) + 1);
                                sprintf(wildcard, "%s", arguments[l]);
                                
                                if(wildcard[strlen(wildcard) - 1] != 10){
                                    wildcard[strlen(wildcard)] = '\0';
                                }else{
                                    wildcard[strlen(wildcard) - 1] = '\0';
                                }

                                int result = glob(wildcard, 0, NULL, &globbuf);
                                if(result == 0){
                                    for(int i = 0; i < globbuf.gl_pathc; i++){
                                        sprintf(args[x], "%s", globbuf.gl_pathv[i]);
                                        x++;
                                    }

                                    globfree(&globbuf);
                                    break;
                                }else{
                                    args[x] = arguments[l];
                                    if(args[x][strlen(args[x]) - 1] != 10){
                                        args[x][strlen(args[x])] = '\0';
                                    }else{
                                        args[x][strlen(args[x]) - 1] = '\0';
                                    }

                                    x++;
                                }
                            }else if(m == strlen(arguments[l] - 1)){
                                sprintf(args[x], "%s", arguments[l]);
                                if(args[x][strlen(args[x]) - 1] != 10){
                                    args[x][strlen(args[x])] = '\0';
                                }else{
                                    args[x][strlen(args[x]) - 1] = '\0';
                                }

                                x++;
                            }
                        }
                    }
                }
                i = 0;
                int in = 0;
                int out = 0;
                char input[64];
                char output[64];

                while(args[i][0] != '\0'){
                    if(strcmp(args[i], "<") == 0){
                        args[i] = NULL;
                        strcpy(input, args[i + 1]);
                        in = 2;
                    }else if(strcmp(args[i], ">") == 0){
                        args[i] = NULL;
                        strcpy(output, args[i + 1]);
                        out = 2;
                    }
                    
                    i++;
                }

                args[i] = NULL;
                
                int pid = fork();
                if(pid == 0){    
                    if(in){
                        int fd0;
                        if((fd0 = open(input, O_RDONLY, 0)) < 0){
                            perror("Unable to read input file");
                            exit(0);
                        }

                        dup2(fd0, STDIN_FILENO);
                        close(fd0);
                    }

                    int terminal = dup(STDOUT_FILENO);
                    
                    if(out){
                        int fd1;
                        if((fd1 = creat(output, 0640)) < 0){
                            perror("Unable to open output file\n");
                            exit(0);
                        }

                        dup2(fd1, STDOUT_FILENO);
                        close(fd1);
                    }
                    if(execv(exec, args) == -1){
                        perror("execv failed");
                    }
                    
                    dup2(terminal, STDOUT_FILENO);
                    close(terminal);
                    exit(0);
                }else{
                    wait(NULL);
                }
                
                memset(arguments, '\0', sizeof(arguments));
                memset(exec, '\0', sizeof(exec));
                free(args);
            }
        }
    }else{ //batch mode
        //search for script file

        //if not found, error

        //if found open file and run commands
    }
}
