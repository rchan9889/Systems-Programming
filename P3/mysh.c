

int main(int arc, char *argv){
    int interactive;
    //if no argument, or file given is from terminal, interactive is set to true
    if(!argv || isatty(argv)){
        interactive = 1;
    }else{
        interactive = 0;
    }

    //interactive mode
    if(interactive){
        printf("Welcome to my shell!");
        while(interactive){
            char command[5];
            scanf("%s", command);
            if(strcmp(command, "cd")){

            }else if(strcmp(command, "pwd")){

            }else if(strcmp(command, "which")){
                
            }else if(strcmp(command, "exit")){
                printf("exiting");
                return 0;
            }else{ //unknown command
            
            }
        }
    }else{ //batch mode
        //search for script file

        //if not found, error

        //if found open file and run commands
    }

}