

int main(int arc, char **argv){
    int interactive;
    if(!argv[0] || isatty(argv[0])){
        interactive = 1;
    }else{
        interactive = 0;
    }

    if(interactive){
        printf("Welcome to my shell!");
    }
}