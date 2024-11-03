#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

typedef struct dict_w {
    char *name;
    int count;
} dict_w;

typedef struct dict_node {
    dict_w word;
    struct dict_node *next;
} dict_node;

dict_node *head = NULL; // This will be a linked list of all of the words, ordered lexicographically



int count_spaces(char *str) {
    int result = 0;

    for (int i = 0; i <= strlen(str); i++) {
        //printf("Testing current letter is: ->%c<-\n", *t);
        if (*(str+i) == ' ' || *(str+i) == '\0') {
            if (i == 0) {
                continue;
            }
            if (*(str+i-1) == ' ') {
                continue;
            }
            result++;
        }
    }
    return result;
}

char **split(const char *input) {
    char s[strlen(input) + 1];
    strcpy(s, input);

    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == 45) { // Current letter is a hyphen
            if (i == 0 || i == strlen(s) - 1) {
                s[i] = ' ';
            }
            else if ((s[i-1] < 65 || ((s[i-1] > 90 && s[i-1] < 97) || s[i-1] > 122))) {
                s[i] = ' ';
            }
            else if ((s[i+1] < 65 || ((s[i+1] > 90 && s[i+1] < 97) || s[i+1] > 122))) {
                s[i] = ' ';
            }
        }
        else if (s[i] == 39) { // Current letter is an apostrophe
            if (!(s[i-1] < 65 || ((s[i-1] > 90 && s[i-1] < 97) || s[i-1] > 122))) {
                s[i] = ' ';
            }
            else if (!(s[i+1] < 65 || ((s[i+1] > 90 && s[i+1] < 97) || s[i+1] > 122))) {
                s[i] = ' ';
            }
        }
        else if ((s[i] < 65 || ((s[i] > 90 && s[i] < 97) || s[i] > 122))) {
                s[i] = ' ';
        }
        
    }

    int num_words = count_spaces(s);
    char **result = malloc(sizeof(char *) * num_words + 1);

    int j = 0;
    
    for (char* word = strtok(s, " "); word; word = strtok(NULL, " "), j++) {
        int word_length = strlen(word);
        
        result[j] = malloc(word_length + 1);
        strcpy(result[j], word);
    }

    result[j] = NULL;
    return result;

}

void updateDict(char **wordList) {
    for (char **s = wordList; *s; s++) {
        if (head == NULL) { // Only happens for very first word
            head = (dict_node*)malloc(sizeof(dict_node));
            head->word.name = malloc(sizeof(char) * sizeof(*s) + 1);
            strcpy(head->word.name, *s);
            head->word.count = 1;
            head->next = NULL;
        }
        else {
            if (strcmp(*s, head->word.name) < 0) { // If current word goes before current head
                dict_node *newHead = (dict_node*)malloc(sizeof(dict_node));
                newHead->word.name = malloc(sizeof(char) * sizeof(*s) + 1);
                strcpy(newHead->word.name, *s);
                newHead->word.count = 1;
                newHead->next = head;
                head = newHead;
            }
            else if (strcmp(*s, head->word.name) == 0) { // If current word is same word as current head
                head->word.count += 1;
            }
            else { // Now we start traversing down the linked list
                for (dict_node *curr = head; curr; curr = curr->next) {
                    if (curr->next == NULL) { // If we've reached end of linked list, just add current word to end
                        dict_node *newNode = (dict_node*)malloc(sizeof(dict_node));
                        newNode->word.name = malloc(sizeof(char) * sizeof(*s) + 1);
                        strcpy(newNode->word.name, *s);
                        newNode->word.count = 1;
                        curr->next = newNode;
                        break;
                    }
                    else {
                        if (strcmp(*s, curr->next->word.name) < 0) { // Current word goes before next word in linked list
                            dict_node *newNode = (dict_node*)malloc(sizeof(dict_node));
                            newNode->word.name = malloc(sizeof(char) * sizeof(*s) + 1);
                            strcpy(newNode->word.name, *s);
                            newNode->word.count = 1;
                            newNode->next = curr->next;
                            curr->next = newNode;
                            break;
                        }
                        else if (strcmp(*s, curr->next->word.name) == 0) { // Current word same as next word in linked list
                            curr->next->word.count += 1;
                            break;
                        }
                    }
                }
            }
        }
        

    }
}

char *get_lines(int fd) {
    char buf[128];
    char *text = NULL;
    int filelength = 0;
    int bytes;
    while ((bytes = read(fd, buf, 128)) > 0) {
        int pos;
        //segstart = 0;
        for (pos = 0; pos < bytes; pos++) {
            if (!buf[pos]) {
                //int seglength = pos - segstart;
                text = realloc(text, filelength + pos + 1);
                memcpy(text + filelength, buf, pos);
                text[filelength + pos] = '\0';
                //updateDict(split(text));
                
                return text;
            }
        }
        text = realloc(text, filelength + 129);
        memcpy(text + filelength, buf, 128);
        filelength += 128;
    }
    if (text) {
        text[filelength] = '\0';
    }
    return text;
}

void traverse(char *path) {
    struct stat sbb;
    stat(path, &sbb);
    if (S_ISREG(sbb.st_mode)) {
        if (strlen(path) < 4) return;
        if (strcmp(path + strlen(path) - 4, ".txt") != 0) {
            return;
        }
        int fd = open(path, O_RDWR);
        char **wordList = split(get_lines(fd));
        updateDict(wordList);
        return;
    }

    int pathlen = strlen(path);
    char *fpath;
    DIR *dp = opendir(path);

    if (!dp) {
        perror(path);
        return;
    }

    struct dirent *de;
    while ((de = readdir(dp)) != NULL) {
        if (de->d_name[0] == '.') {
            continue;
        }
        

        printf("%s\n", de->d_name);

        int namelen = strlen(de->d_name);
        fpath = (char *) malloc(pathlen + 1 + namelen + 1);
        memcpy(fpath, path, pathlen);
        fpath[pathlen] = '/';
        memcpy(fpath + pathlen + 1, de->d_name, namelen + 1);
        printf("%s\n", fpath);

        struct stat sb;
        stat(fpath, &sb);
        if (S_ISDIR(sb.st_mode)) {
            traverse(fpath);
        }
        else if (S_ISREG(sb.st_mode)) {
            if (strlen(de->d_name) < 4) continue;
            if (strcmp(de->d_name + strlen(de->d_name) - 4, ".txt") != 0) {
                continue;
            }
            int fd = open(fpath, O_RDWR);
            char **wordList = split(get_lines(fd));
            updateDict(wordList);
        }
        else {
            return;
        }

        free(fpath);
    }

    closedir(dp);
}
int main(int argc, char **argv){
    for (int i = 0; i < argc; i++) {
        traverse(argv[i]);
    }


    dict_node *sortedHead = NULL; //We've sorted lexicographically, but now we need to sort numerically too

    for (dict_node *curr = head; curr; curr = curr->next) {
        if (sortedHead == NULL) { // Only happens for very first word
            sortedHead = (dict_node*)malloc(sizeof(dict_node));
            sortedHead->word.name = malloc(sizeof(char) * sizeof(curr->word.name) + 1);
            strcpy(sortedHead->word.name, curr->word.name);
            sortedHead->word.count = curr->word.count;
            sortedHead->next = NULL;
        }
        else {
            if (curr->word.count > sortedHead->word.count) { // If current word goes before current head
                dict_node *newHead = (dict_node*)malloc(sizeof(dict_node));
                newHead->word.name = malloc(sizeof(char) * sizeof(curr->word.name) + 1);
                strcpy(newHead->word.name, curr->word.name);
                newHead->word.count = curr->word.count;
                newHead->next = sortedHead;
                sortedHead = newHead;
            }
            else { // Now we start traversing down the sorted linked list. Since we've already sorted lexicographically, we can just attach each node to end of group of nodes with same count
                for (dict_node *sortedCurr = sortedHead; sortedCurr; sortedCurr = sortedCurr->next) {
                    if (sortedCurr->next == NULL) { // If we've reached end of sorted linked list, just add current word to end
                        dict_node *newNode = (dict_node*)malloc(sizeof(dict_node));
                        newNode->word.name = malloc(sizeof(char) * sizeof(curr->word.name) + 1);
                        strcpy(newNode->word.name, curr->word.name);
                        newNode->word.count = curr->word.count;
                        sortedCurr->next = newNode;
                        break;
                    }
                    
                    if (curr->word.count > sortedCurr->next->word.count) { // Current word goes before next word in linked list
                        dict_node *newNode = (dict_node*)malloc(sizeof(dict_node));
                        newNode->word.name = malloc(sizeof(char) * sizeof(*curr->word.name) + 1);
                        strcpy(newNode->word.name, curr->word.name);
                        newNode->word.count = curr->word.count;
                        newNode->next = sortedCurr->next;
                        sortedCurr->next = newNode;
                        break;
                    }
                        
                    
                }
            }
        }
    }
    
    // Now we can finally print while also freeing node by node after
    for (dict_node *sortedCurr = sortedHead; sortedCurr; sortedCurr = sortedCurr;) {
        printf("%s %d\n", sortedCurr->word.name, sortedCurr->word.count);
        dict_node *nextSortedNode = sortedCurr->next;
        free(sortedCurr->word.name);
        free(sortedCurr);
        sortedCurr = nextSortedNode;
    }

    
    for (dict_node *tempNode = head; tempNode; tempNode = tempNode;) {
        dict_node *nextNode = tempNode->next;
        free(tempNode->word.name);
        free(tempNode);
        tempNode = nextNode;
    }
}


