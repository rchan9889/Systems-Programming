#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <stdlib.h>

int main(int arc, char *argv){
    int interactive;
    FILE *fp;
    int fd;

    fp = fopen(argv, "r");

    if(fp == NULL){ //no argument
        interactive = 1;
    }else{
        fd = fileno(fp);
    }
    if(!interactive && !isatty(fd)){
        interactive = 0;
    }else{
        interactive = 1;
    }

    //interactive mode
    if(interactive){
        printf("Welcome to my shell!\n");
        while(interactive){
            printf("mysh> ");
            char command[200];
            fgets(command, sizeof(command), stdin);
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
                }
                memset(path, 0, sizeof(path));
            }else if(command[0] == 'p' && command[1] == 'w' && command[2] == 'd'){ //pwd
                char cwd[PATH_MAX];
                getcwd(cwd, sizeof(cwd));
                printf("%s\n", cwd);
            }else if(command[0] == 'w' && command[1] == 'h' && command[2] == 'i' && command[3] == 'c' && command[4] == 'h' && command[5] == ' '){ //which
        	    char path[PATH_MAX];
                int i = 6;
                while(command[i]){
		            //printf("Testing: %d\n", i);	
                    if (command[i] != 10) {
                        path[i - 6] = command[i];
		            }
		            i++;
                }

                char cwd[PATH_MAX];
                getcwd(cwd, sizeof(cwd));
                char *filename = malloc(sizeof(cwd) + 1 + sizeof(path) + 1); // blah/blah + / + testfile + \0
                    
		        strcat(filename, cwd);
                strcat(filename, "/");
                strcat(filename, path);
                if (access(filename, F_OK) != -1) {
                    printf("%s\n", filename);
                }
                /*
                char* dirOne = malloc(sizeof(char) * (15 + sizeof(path) + 1));
                strcat(dirOne, "/usr/local/bin/");
                strcat(dirOne, path);
                    
                char* dirTwo = malloc(sizeof(char) * (9 + sizeof(path) + 1));
                strcat(dirTwo, "/usr/bin/");
                strcat(dirTwo, path);
                    
                char* dirThree = malloc(sizeof(char) * (5 + sizeof(path) + 1));
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
                */       
            }else if(command[0] == 'e' && command[1] == 'x' && command[2] == 'i' && command[3] == 't'){ //exit
                printf("exiting\n");
                int i = 4;
                while(command[i]){
                    printf("%c", command[i]);
                }
                return 0;
            }else if(command[0] == '.' && command[1] == '/' && command[2] == ' '){ //./

            }else{
            
            }        
        }
    }else{ //batch mode
        //search for script file
        char command[200];

        if (fp == NULL) {
            printf("Failed to open file.\n");
            return 1;
        }
        //if found open file and run commands
        while (fgets(command, sizeof(command), fp) != NULL) {
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
                }
                memset(path, 0, sizeof(path));
            }else if(command[0] == 'p' && command[1] == 'w' && command[2] == 'd'){ //pwd
                char cwd[PATH_MAX];
                getcwd(cwd, sizeof(cwd));
                printf("%s\n", cwd);
            }else if(command[0] == 'w' && command[1] == 'h' && command[2] == 'i' && command[3] == 'c' && command[4] == 'h' && command[5] == ' '){ //which
        	    char path[PATH_MAX];
                int i = 6;
                while(command[i]){
		            //printf("Testing: %d\n", i);	
                    if (command[i] != 10) {
                        path[i - 6] = command[i];
		            }
		            i++;
                }

                char cwd[PATH_MAX];
                getcwd(cwd, sizeof(cwd));
                char *filename = malloc(sizeof(cwd) + 1 + sizeof(path) + 1); // blah/blah + / + testfile + \0
                    
		        strcat(filename, cwd);
                strcat(filename, "/");
                strcat(filename, path);
                if (access(filename, F_OK) != -1) {
                    printf("%s\n", filename);
                }
                /*
                char* dirOne = malloc(sizeof(char) * (15 + sizeof(path) + 1));
                strcat(dirOne, "/usr/local/bin/");
                strcat(dirOne, path);
                    
                char* dirTwo = malloc(sizeof(char) * (9 + sizeof(path) + 1));
                strcat(dirTwo, "/usr/bin/");
                strcat(dirTwo, path);
                    
                char* dirThree = malloc(sizeof(char) * (5 + sizeof(path) + 1));
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
                */       
            }else if(command[0] == 'e' && command[1] == 'x' && command[2] == 'i' && command[3] == 't'){ //exit
                printf("exiting\n");
                int i = 4;
                while(command[i]){
                    printf("%c", command[i]);
                }
                return 0;
            }else if(command[0] == '.' && command[1] == '/' && command[2] == ' '){ //./

            }else{
            
            }
        }
        

        
    }

}
