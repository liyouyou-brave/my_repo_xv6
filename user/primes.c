#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void handle_func(int fd[2])
{
    int prime;
    close(fd[1]);
    read(fd[0], &prime, 4);
    printf("prime %d\n", prime);
    int num;
    // 尝试看看还能不能接收到
    if (read(fd[0], &num, 4))
    {
        int newfd[2];
        pipe(newfd);
        if (fork() == 0)
        {
            handle_func(newfd);
        }
        else
        {
            close(newfd[0]);
            if (num % prime) {
                if(write(newfd[1], &num, 4) != 4) {
                    printf("write error\n");
                    exit(1);
                }
            }
                
            while (read(fd[0], &num, 4))
            {
                if (num % prime)
                {
                    if(write(newfd[1], &num, 4) != 4) {
                        printf("write error\n");
                        exit(1);
                    }
                }
            }
            close(fd[0]);
            close(newfd[1]);
            wait(0);
            exit(0);
        }
    }
}

int main(int argc, char *argv)
{
    if (argc > 1)
    {
        printf("usage: primes\n");
        exit(1);
    }
    int fd[2];
    pipe(fd);
    if (fork() == 0)
    {
        handle_func(fd);
    }
    else
    {
        close(fd[0]);
        for (int i = 2; i <= 35; i++)
        {
            if (write(fd[1], &i, 4) != 4) {
                printf("write error\n");
                exit(1);
            }
            
        }
        close(fd[1]);
        wait(0);
        exit(0);
    }
    return 0;
}