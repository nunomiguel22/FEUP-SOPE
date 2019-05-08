#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h> 
#include <errno.h>
#include <time.h>

#include "constants.h"
#include "general_aux.h"


void create_fifo (char* fifo_name){
    if (mkfifo(fifo_name, 0660) < 0){
        if (errno==EEXIST) printf("FIFO '%s' already exists\n", fifo_name);
        else printf("Can't create FIFO '%s'\n", fifo_name);
        exit(EXIT_FAILURE);
    }
}

void remove_fifo (char *fifo_name){
    if (unlink(fifo_name)<0){
        printf("Error when destroying FIFO '%s'\n", fifo_name);
        exit(EXIT_FAILURE);
    }
}

void run_pipe_command(char *command ,char *result) {
    int pipeStatus;
    FILE *fpout;
    fpout = popen(command, "r");
    if (fpout == NULL) {
        fprintf(stderr, "Error opening pipe");
        exit(EXIT_FAILURE);
    }
    fgets(result, MAXLINE, fpout);
    pipeStatus = pclose(fpout);
    if(pipeStatus == -1) {
        fprintf(stderr, "Error closing pipe");
    }
}

bool is_valid_password(char *password){
    if (strlen(password) < MIN_PASSWORD_LEN)
        return false;    
    if (strlen(password) > MAX_PASSWORD_LEN)
        return false;
    return true;
}

int get_string_arguments(char* arguments, char*argv[]){
    int argc = 0;
    char *ptr = strtok(arguments, " ");
    while (ptr != NULL){
        argv[argc] = ptr;
        ptr = strtok(NULL, " ");
        ++argc;
    }
    return argc;
}

void generate_sha256_hash(char *password, char salt[], char hash[]){  
    char sha256sum_command[MAXLINE];
    sprintf(sha256sum_command, "echo -n \"%s%s\" | sha256sum", password, salt);
    char pipe_result[MAXLINE];
    run_pipe_command(sha256sum_command, pipe_result);
    char *temp_ptr = strtok(pipe_result, " ");
    strcpy(hash, temp_ptr);
    hash[HASH_LEN] = '\0';
}

void generate_password_salt(char salt[]){
    for (int i = 0; i < SALT_LEN; ++i){
        int randomValue = rand() % 15;
        sprintf(&salt[i], "%x", randomValue);
    }
    salt[SALT_LEN] = '\0';
}