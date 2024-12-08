#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// fork() 返回值：子进程返回0，父进程返回子进程的pid
// pipe出两个端口: fd[0](read), fd[1](write), 由于管道在工作时是单向的, 所以在write/read的时候, 需要close掉read/write
int
main(int argc, char* argv[])
{
    if (argc > 1)
    {
        printf("usage: pingpong\n");
        exit(1);
    }
    int fd[2];
    pipe(fd);
    if (fork() == 0)
    { // 子进程
        char buf[2];
        if (read(fd[0], buf, 1) != 1) {
            printf("Child read error!\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        close(fd[0]);
        char message[2];
        message[0] = 'a';
        if (write(fd[1], message, 1) != 1) {
            printf("Child write error!\n");
            exit(1);
        }
        close(fd[1]);
    }
    else
    { // 父进程
        char message[2];
        message[0] = 'a';
        if (write(fd[1], message, 1) != 1) {
            printf("Parent write error!\n");
            exit(1);
        }
        close(fd[1]);
        // 等待子进程结束
        wait(0);
        char buf[2];
        if (read(fd[0], buf, 1) != 1) {
            printf("Parent read error!\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        close(fd[0]);
    }
    exit(0);
}