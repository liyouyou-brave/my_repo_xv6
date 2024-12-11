#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#define DEBUG 1

// #define debug(codes) if(DEBUG) {codes;}
char*
readline()
{
    char* buf = malloc(1024);
    int n = 0;
    while (read(0, buf + n, 1)) {
        if (n >= 1023) {
            printf("argument too long\n");
            exit(0);
        }
        if (buf[n] == '\n' || buf[n] == '\0') {
            buf[n] = '\0';
            break;
        }
        n++;
    }   
    if (n == 0) {
        free(buf);
        return 0;
    }
    return buf;
}
int
main(int argc, char const *argv[])
{
    char* cmd = malloc(strlen(argv[1]) + 1);
    strcpy(cmd, argv[1]);
    char* new_argv[MAXARG];
    int new_argc = argc;
    int i;
    for (i = 1; i < argc; i++) {
        new_argv[i - 1] = malloc(strlen(argv[i]) + 1);
        strcpy(new_argv[i - 1], argv[i]);
    }
    char* new_buf;
    while ((new_buf = readline()) != 0) {
        new_argv[new_argc - 1] = malloc(strlen(new_buf));
        strcpy(new_argv[new_argc - 1], new_buf);
        if (fork() == 0) {
            exec(cmd, new_argv);
        }
        else {
            wait(0);
        }
    }
    free(cmd);
    free(new_argv);
    exit(0);
    return 0;
}