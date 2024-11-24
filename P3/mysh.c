#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>

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
            char command[40];
            fgets(command, sizeof(command), stdin);
            if(command[0] == 'c' && command[1] == 'd' && command[2] == ' '){ //cd fix, I dunno how
                char path[PATH_MAX];
                int i = 3;
                while(command[i]){
                    if (command[i] != 10) {
                        path[i - 3] = command[i];
                    }
                    i++;
                }
                printf("%s\n", path);

                if(chdir(path) == -1){
                    printf("cd: No such file or directory\n");
                }else{
                    char cwd[PATH_MAX];
                    getcwd(cwd, sizeof(cwd));
                    printf("%s\n", cwd);
                }
                memset(path, 0, sizeof(path));
            }else if(command[0] == 'p' && command[1] == 'w' && command[2] == 'd'){ //pwd
                char cwd[PATH_MAX];
                getcwd(cwd, sizeof(cwd));
                printf("%s\n", cwd);
            }else if(command[0] == 'w' && command[1] == 'h' && command[2] == 'i' && command[3] == 'c' && command[4] == 'h' && command[5] == ' '){ //which
                
            }else if(command[0] == 'e' && command[1] == 'x' && command[2] == 'i' && command[3] == 't'){ //exit
                printf("exiting\n");
                return 0;
            }else if(command[0] == '.' && command[1] == '/' && command[2] == ' '){ //./

            }else{
            
            }
            
        }
    }else{ //batch mode
        //search for script file

        //if not found, error

        //if found open file and run commands
    }

}
