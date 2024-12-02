#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <glob.h>

char* where(char path[]) {
    printf("path is |%s|\n", path);
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
                    
    printf("dirOne = %s\n", dirOne);
    printf("dirTwo = _%s_\n", dirTwo);
    printf("dirThree = _%s_\n", dirThree);
    
    
    if (access(dirOne, F_OK) != -1) {
   	char* answerOne = malloc((15 + strlen(path) + 1) * sizeof(char));
	strcpy(answerOne, dirOne);
	return answerOne;
    } else if (access(dirTwo, F_OK) != -1) {
        char* answerTwo = malloc((9 + strlen(path) + 1) * sizeof(char));
        strcpy(answerTwo, dirTwo);
	//printf("DrTwo\n");
	return answerTwo;
    } else if (access(dirThree, F_OK) != -1) {
        char* answerThree = malloc((5 + strlen(path) + 1) * sizeof(char));
        strcpy(answerThree, dirThree);
	//printf("DrThee\n");
	return answerThree;
    } else {
	return NULL;
    }
                
}

int main(int arc, char *argv){
    int interactive;
    // FILE *fp;
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
            char* command = malloc(200 * sizeof(char));
            // memset(command, 0, sizeof(command));
            // fgets(command, sizeof(command), stdin);
            if (read(0, command, 200) == 0) {
                printf("Something wrong has occured. Exiting program now.\n");
                return 1;
            }
	    printf("Strlen for command is |%ld|\n", strlen(command));	
            printf("Command string is |%s|\n", command);
            if(command[0] == 'c' && command[1] == 'd' && command[2] == ' '){ //cd
                char path[PATH_MAX];
                int i = 3;
                while(command[i]){
		    //printf("Testing: %d\n", i);	
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
		        //printf("Testing");
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
                        printf("%s\n", filename);
                    }
                    free(filename);
                    memset(cwd, 0, sizeof(cwd));
                }
                memset(path, 0, sizeof(path));
            }else if(command[0] == 'p' && command[1] == 'w' && command[2] == 'd'){ //pwd
                char cwd[PATH_MAX];
                getcwd(cwd, sizeof(cwd));
                printf("%s\n", cwd);
                memset(cwd, 0, sizeof(cwd));
            }else if(command[0] == 'w' && command[1] == 'h' && command[2] == 'i' && command[3] == 'c' && command[4] == 'h' && command[5] == ' '){ //which
        	char path[PATH_MAX];
                int i = 6;
                memset(path, 0, sizeof(path));
		while(command[i]){
	            //printf("Testing: %d\n", i);	
                    if (command[i] != 10) {
                        path[i - 6] = command[I];
		    }
		    i++;
                }

                /*
                char cwd[PATH_MAX];
                getcwd(cwd, sizeof(cwd));
                char *filename = malloc(sizeof(cwd) + 1 + sizeof(path) + 1); // blah/blah + / + testfile + \0
                    
		strcat(filename, cwd);
                strcat(filename, "/");
                strcat(filename, path);
                printf("This is a test print statement. Current filename = |%s|\n", filename);
                if (access(filename, F_OK) != -1) {
                    printf("%s exists within the current directory.\n", path);
                    printf("%s\n", filename);
                }
                else {
                    printf("Something went wrong. Filename: |%s|\n", filename)
                }
                free(filename);
                memset(cwd, 0, sizeof(cwd));
                memset(path, 0, sizeof(path));
                */

                char* result = where(path);
                if (result == NULL) {
                    printf("%s wasn't found in any of the three directories.\n", path);
                }
                else {
                    printf("%s\n", result);
                }
                free(result);
                //memset(path, 0, sizeof(path));
		
            }else if(command[0] == 'e' && command[1] == 'x' && command[2] == 'i' && command[3] == 't'){ //exit
                printf("exiting\n");
		int i = 4;
                /*
		while(command[i]){
                    printf("%c", command[i]);
		}
		*/
                return 0;
            }else{ //bare names
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

                //printf("%s", arguments[2]);
                
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
                            //printf("%s", file);
                            getcwd(exec, (sizeof(exec)));
                            //printf("%s", exec);
                            strcat(exec, file);
                            exec[strlen(exec)] = '\0';
                            //printf("%s", exec);
                        }else{
                            if(arguments[l][0] != '/'){
                                if(arguments[l][strlen(arguments[l]) - 1] != 10){
                                    arguments[l][strlen(arguments[l])] = 0;
                                }else{
                                    arguments[l][strlen(arguments[l]) - 1] = 0;
                                }
                                //arguments[l][strlen(arguments[l]) - 1] = 'b';
                                //printf("%s", arguments[l]);
                                sprintf(exec, "%s", where(arguments[l]));
                                //printf("%s\n", exec);
                            }else{
                                sprintf(exec, "%s", arguments[l]);
                            }
                            //printf("%s\n", exec);
                            
                            //printf("%c\n", exec[strlen(exec) - 1]);
                            //x++;
                        }
                        sprintf(args[0], "%s", exec);
                        //printf("%s", exec);
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
                                        //printf("%s\n", globbuf.gl_pathv[i]);
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
                                //printf("%s ", args[x]);
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
                //("%s", args[0]);
                while(args[i][0] != '\0'){
                    //printf("%s", args[i]);
                    if(strcmp(args[i], "<") == 0){
                        args[i] = NULL;
                        strcpy(input, args[i + 1]);
                        in = 2;
                    }

                    if(strcmp(args[i], ">") == 0){
                        args[i] = NULL;
                        strcpy(output, args[i + 1]);
                        out = 2;
                    }
                    i++;
                }

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

                args[i] = NULL;
                
                int pid = fork();
                if(pid == 0){    
                    if(execv(exec, args) == -1){
                        perror("execv failed");
                    }
                    exit(0);
                }else{
                    wait(NULL);
                }

                dup2(terminal, STDOUT_FILENO);
                close(terminal);
                
                memset(arguments, '\0', sizeof(arguments));
                memset(exec, '\0', sizeof(exec));
                free(args);
                //free(exec);
            }
        }
    }else{ //batch mode
        //search for script file
        char command[200];

        if (fd == -1) {
            printf("Failed to open file.\n");
            return 1;
        }
        //if found open file and run commands
        int moreToRead = 1;
            
        while (moreToRead) {
            char* command = malloc(200 * sizeof(char));
            char ch = 0;
            while (1) {
                if (read(fd, &ch, 1) == 0) {
                    moreToRead = 0;
                    printf("We have reached the end of the file.\n");
                    break;
                }
                else if (ch == 10){
                    printf("We have reached a newline.\n");
                    break;
                }
                else {
                    strncat(command, &ch, 1);
                    printf("Current value of variable command: %s\n", command);
                }
            }
            printf("Command string is %s", command);
            //printf("Something wrong has occured. Exiting program now.\n");
        
            if(command[0] == 'c' && command[1] == 'd' && command[2] == ' '){ //cd fix, I dunno how
                char path[PATH_MAX];
                int i = 3;
                while(command[i]){
		            //printf("Testing: %d\n", i);	
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
		                //printf("Testing");
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
            }else if(command[0] == 'w' && command[1] == 'h' && command[2] == 'i' && command[3] == 'c' && command[4] == 'h' && command[5] == ' '){ //which
        	char path[PATH_MAX];
                int i = 6;
		memset(path, 0, sizeof(path));
                while(command[i]){
		            //printf("Testing: %d\n", i);	
                    if (command[i] != 10) {
                        path[i - 6] = command[I];
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
                
                

                /*
                char* dirOne = malloc(sizeof(char) * (15 + sizeof(path)));
                strcat(dirOne, "/usr/local/bin/");
                strcat(dirOne, path);
                    
                char* dirTwo = malloc(sizeof(char) * (9 + sizeof(path)));
                strcat(dirTwo, "/usr/bin/");
                strcat(dirTwo, path);
                    
                char* dirThree = malloc(sizeof(char) * (5 + sizeof(path)));
                strcat(dirThree, "/bin/");
                strcat(dirThree, path);
                    
                printf("dirOne = _%s_\n", dirOne);
                printf("dirTwo = _%s_\n", dirTwo);
                printf("dirThree = _%s_\n", dirThree);
                if (access(dirOne, F_OK) != -1) {
                    printf("%s\n", dirOne);
                } else if (access(dirTwo, F_OK) != -1) {
                    printf("%s\n", dirTwo);
                } else if (access(dirThree, F_OK) != -1) {
                    printf("%s\n", dirThree);
                }
                free(dirOne);
                free(dirTwo);
                free(dirThree);   
                memset(path, 0, sizeof(path)); 
		*/
            }else if(command[0] == 'e' && command[1] == 'c' && command[2] == 'h' && command[3] == 'o') {
                char text[PATH_MAX];
                int i = 5;
                while(command[i]){
		            //printf("Testing: %d\n", i);	
                    if (command[i] != 10) {
                        text[i - 5] = command[i];
		            }
		            i++;
                }
                printf("%s\n", text);
                memset(text, 0, sizeof(text));  
            
            }else if(command[0] == 'e' && command[1] == 'x' && command[2] == 'i' && command[3] == 't'){ //exit
                printf("exiting\n");
                int i = 4;
                while(command[i] != '\0'){
                    printf("%c", command[i]);
                    i++;
                }
                return 0;
            }else if((command[0] == '.' && command[1] == '/') || (command[0] == '/')){ //./ or /
                if(command[0] == '/'){
                    char path[PATH_MAX];
                    int i = 0;
                    while(command[i] != 10){
                        path[i] = command[i];
                        i++;
                    }

                    printf("%s\n", path);

                    int result = system(path);

                    if(result){
                        printf("Failed to execute: %s", path);
                    }
                    memset(path, 0, sizeof(path));
                }else{
                    char file[PATH_MAX];
                    int i = 2;
                    while(command[i] != 10){
                        file[i - 2] = command[i];
                        i++;
                    }
                    char path[PATH_MAX];
                    getcwd(path, sizeof(path));
                    strcat(path, "/");
                    strcat(path, file);

                    printf("%s\n", path);

                    int result = system(path);

                    if(result){
                        printf("Failed to execute.");
                    }
                    memset(file, 0, sizeof(file));
                    memset(path, 0, sizeof(path));
                }
                printf("\n");
            }else{ //bare names
            
            }
        }
        

        
    }

}

