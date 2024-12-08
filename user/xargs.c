#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#define DEBUG 1

// #define debug(codes) if(DEBUG) {codes;}

int 
readline(char** new_argv, int cur) {
    char buf[1024];
    int n = 0;
    while (read(0, buf + n, 1)) {
        if (buf[n] == '\n') {
            break;
        }
        n++;
    }
    buf[n] = 0;
    if (n == 0) 
        return 0;
    int offset = 0;
    while (offset < n) {
        new_argv[cur] = buf + offset;
        while (buf[offset] != ' ' && offset < n) {
            offset++;
        }
        while (buf[offset] == ' ' && offset < n) {
            offset++;
        }
        cur++;
    }
    return cur;
}


int
main(int argc, char* argv[])
{
    // debug(printf("argc: %d\targv[0]: %s\targv[1]: %s\targv[2]: %s\n", argc, argv[0], argv[1], argv[2]))
    
    char* cmd = malloc(strlen(argv[1]) + 1);
    strcpy(cmd, argv[1]);
    char* new_argv[MAXARG];
    // printf("%s\n", cmd);
    for (int i = 1; i < argc; i++) {
        new_argv[i - 1] = malloc(sizeof(argv[i]) + 1);
        strcpy(new_argv[i - 1], argv[i]);
        // printf(new_argv[i - 1]);
    }
    int new_argc = argc - 1;
    int cur_argc;
    while ((cur_argc = readline(new_argv, new_argc)) != 0) {
        new_argv[cur_argc] = 0;
        if (fork() == 0) {
            exec(cmd, new_argv);
        }
        else {
            wait(0);
        }
    }
    
    exit(0);
    return 0;
}