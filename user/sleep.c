#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("usage: sleep <time>\n");
        exit(1);
    }
    int sleep_time = atoi(argv[1]);
    if (sleep_time < 0) {
        printf("sleep time should bigger than zero\n");
        exit(1);
    }
    sleep(sleep_time * 10);
    exit(0);
    return 0;
}